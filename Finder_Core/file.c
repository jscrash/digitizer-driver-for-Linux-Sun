/* DEC/CMS REPLACEMENT HISTORY, Element FILE.C */
/* *1    13-NOV-1990 11:25:44 JULIAN "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element FILE.C */
/* DEC/CMS REPLACEMENT HISTORY, Element FILE.C */
/* *2    25-SEP-1990 05:59:42 JULIAN "(SPR -1) RMS file description language manipulation utility" */
/* *1    25-SEP-1990 05:59:14 JULIAN "RMS file description language manipulation utility" */
/* DEC/CMS REPLACEMENT HISTORY, Element FILE.C */
/* FILE.C, a utility to modify file characteristics.
 
   Written by Joe Meadows Jr, at the Fred Hutchinson Cancer Research Center
   BITNET: JOE@FHCRCVAX
   PHONE: (206) 467-4970
 
   There are no restrictions on this code, you may sell it, include it with
   any commercial package, or feed it to a whale.. However, I would appreciate
   it if you kept this comment in the source code so that anyone receiving
   this code knows who to contact in case of problems. Note that I do not
   demand this condition..
 */
/* Modifications:
     05-OCT-1987
     changed calls to strcmp() into strncmp() to fix problem with
     not being able to abbreviate keywords in /TYPE and /ORGANIZATION
     15-SEP-1987
     modified handling of qualifiers, move check for /OUTPUT outside of
     loop, get values of all other qualifiers before looping through
     wildcarded filenames.
     22-AUG-1987
     Bugfixes (argh!). Fixed output of /NOEXACT, added "-" to first
     line of output. Handle list of file names, as documented.. Make sure
     all lines are under 80 characters, so mailers don't barf.
     25-Jan-1989
     Add call to init_cli, so it can be defined as foreign command
     or a real command.
     26-SEP-1989
     Fixed handling of efblk and hiblk w.r.t. large files (> 64k blocks)
     06-OCT-1989
     Added /PROTECTION qualifier!
     10-OCT-1989
     added "-" to end of /protection output (argh!)
     11-OCT-1989
     Added /OWNER qualifier
     13-APR-1990 (Ned Freed)
     Fixed problem with local negation of qualifiers
*/
 
#include <descrip.h>
#include <rms.h>
#include <stdio.h>
#include <iodef.h>
#ifndef __GNUC__
#  include <climsgdef.h>
#else
#  define CLI$_NEGATED 0X000381F8
#  define CLI$_LOCNEG  0X00038230
#endif
 
#include <atrdef.h>
#include <fibdef.h>
#include "fatdef.h"
#include "fchdef.h"
#include "fjndef.h"
#include "cld.h"
 
#define RTYPE fat$r_rtype_overlay.fat$r_rtype_bits
#define RATTRIB fat$r_rattrib_overlay.fat$r_rattrib_bits
#define EFBLK fat$r_efblk_overlay.fat$r_efblk_fields
#define HIBLK fat$r_hiblk_overlay.fat$r_hiblk_fields
 
char *cvt_time();
unsigned int change();
unsigned int antichange();
 
#ifndef __GNUC__
  globalvalue FILE_CLD;
#else
# define FILE_CLD 0
#endif
 
main()
{
  static struct FAB Fab;
  static struct NAM Nam;
  static struct fibdef Fib; /* short fib */
 
  static struct dsc$descriptor FibDesc =
    {sizeof(Fib),DSC$K_DTYPE_Z,DSC$K_CLASS_S,&Fib};
  static struct dsc$descriptor_s DevDesc =
    {0,DSC$K_DTYPE_T,DSC$K_CLASS_S,&Nam.nam$t_dvi[1]};
  static struct fatdef Fat;
  static union {
    struct fchdef fch;
    long int dummy;
  } uchar;
  static struct fjndef jnl;
  static long int Cdate[2],Rdate[2],Edate[2],Bdate[2];
  static short int revisions;
  static unsigned short uic[2];
  static union {
    unsigned short int value;
    struct {
      unsigned system : 4;
      unsigned owner : 4;
      unsigned group : 4;
      unsigned world : 4;
    } bits;
  } prot;
 
  static struct atrdef Atr[] = {
    sizeof(Fat),ATR$C_RECATTR,&Fat,        /* record attributes */
    sizeof(uchar),ATR$C_UCHAR,&uchar,      /* File characteristics */
    sizeof(Cdate),ATR$C_CREDATE,&Cdate[0], /* Creation date */
    sizeof(Rdate),ATR$C_REVDATE,&Rdate[0], /* Revision date */
    sizeof(Edate),ATR$C_EXPDATE,&Edate[0], /* Expiration date */
    sizeof(Bdate),ATR$C_BAKDATE,&Bdate[0], /* Backup date */
    sizeof(revisions),ATR$C_ASCDATES,&revisions, /* number of revisions */
    sizeof(prot),ATR$C_FPRO,&prot,         /* file protection  */
    sizeof(uic),ATR$C_UIC,&uic,            /* file owner */
    sizeof(jnl),ATR$C_JOURNAL,&jnl,        /* journal flags */
    0,0,0
  } ;
 
  static char EName[NAM$C_MAXRSS];
  static char RName[NAM$C_MAXRSS];
  static struct dsc$descriptor_s FileName =
    {0,DSC$K_DTYPE_T,DSC$K_CLASS_S,0};
  static struct dsc$descriptor_s string = {0,DSC$K_DTYPE_T,DSC$K_CLASS_S,0};
  static short int DevChan;
  static short int iosb[4];
 
  static long int i,j,status;
  static char *retval;
  static short int output = 0;
 
  status = init_cli(FILE_CLD,"FILE");
 
  if (cli_present("OUTPUT") & 1) {
    /* see if they want output to go to a file or to SYS$OUTPUT */
    status = cli_get_value("OUTPUT",&retval);
    if (status & 1)
      freopen(retval,"w",stdout,"rfm=var","rat=cr");
    output = 1;
  };
 
  while (cli_get_value("P1",&retval) & 1) {
    /* initialize RMS structures, we need a NAM to retrieve the FID */
    Fab = cc$rms_fab;
    Fab.fab$l_fna = retval ; /* name of file */
    Fab.fab$b_fns = strlen(retval);
    Fab.fab$l_nam = &Nam; /* FAB has an associated NAM */
    Nam = cc$rms_nam;
    Nam.nam$l_esa = &EName; /* expanded filename */
    Nam.nam$b_ess = sizeof(EName);
    Nam.nam$l_rsa = &RName; /* resultant filename */
    Nam.nam$b_rss = sizeof(RName);
 
    /* do $PARSE and $SEARCH here */
    status = sys$parse(&Fab);
    if (!(status & 1)) return(status);
 
    /* search for the first file.. If none signal error */
    status = sys$search(&Fab);
    if (!(status & 1)) return(status);
 
    while (status & 1) {
      /* dummy cli_get_value, used merely to reset context */
      status = cli_get_value("OUTPUT",&retval);
 
      /* initialize Device name length, note that this points into the NAM
         to get the device name filled in by the $PARSE, $SEARCH services */
      DevDesc.dsc$w_length = Nam.nam$t_dvi[0];
 
      status = sys$assign(&DevDesc,&DevChan,0,0);
      if (!(status & 1)) return(status);
 
      FileName.dsc$a_pointer = Nam.nam$l_name;
      FileName.dsc$w_length = Nam.nam$b_name+Nam.nam$b_type+Nam.nam$b_ver;
 
      /* Initialize the FIB */
      for (i=0;i<3;i++)
        Fib.fib$r_fid_overlay.fib$w_fid[i]=Nam.nam$w_fid[i];
      for (i=0;i<3;i++)
        Fib.fib$r_did_overlay.fib$w_did[i]=Nam.nam$w_did[i];
 
      /* Use the IO$_ACCESS function to return info about the file */
      /* Note, used this way, the file is not opened, and the expiration */
      /* and revision dates are not modified */
      status = sys$qiow(0,DevChan,IO$_ACCESS,&iosb,0,0,
                        &FibDesc,&FileName,0,0,&Atr,0);
      if (!(status & 1)) return(status);
      status = iosb[0];
      if (!(status & 1)) return(status);
 
      if (output) {
 
        /* if it's /noexact, use only name & type part of the result name */
        /* also fudge the resultant file name to have null termination */
        if (cli_present("EXACT") & 1) {
          i = 0;
          j = Nam.nam$b_rsl;
        }
        else {
          i = Nam.nam$b_node + Nam.nam$b_dev + Nam.nam$b_dir;
          j = Nam.nam$b_name+Nam.nam$b_type;
        }
        printf("$ FILE %.*s-\n",j,&RName[i] );
 
        /* print protection here */
        printf("  /%s=(S%s%s,O%s%s,G%s%s,W%s%s) -\n",t_protection,
             (prot.bits.system != 15)?":":"", t_fileprot[prot.bits.system],
             (prot.bits.owner != 15)?":":"", t_fileprot[prot.bits.owner],
             (prot.bits.group != 15)?":":"", t_fileprot[prot.bits.group],
             (prot.bits.world != 15)?":":"", t_fileprot[prot.bits.world]);
        printf("  /%s=[%o,%o] -\n",t_owner,uic[1],uic[0]);
        printf("  /%s=%s",t_rtype,ta_rtype[Fat.RTYPE.fat$v_rtype]);
        printf("  /%s=%s -\n",t_fileorg,ta_fileorg[Fat.RTYPE.fat$v_fileorg]);
        printf("  /%s=(%s%s,%s%s,%s%s,%s%s) -\n",
          t_attr,
          Fat.RATTRIB.fat$v_fortrancc?"":"NO",t_fortrancc,
          Fat.RATTRIB.fat$v_impliedcc?"":"NO",t_impliedcc,
          Fat.RATTRIB.fat$v_printcc?"":"NO",t_printcc,
          Fat.RATTRIB.fat$v_nospan?"NO":"",t_nospan);
 
        printf("  /%s=(%s%s,%s%s,%s%s,%s%s, -\n",
          t_char,
          uchar.fch.fch$v_badacl?"":"NO",t_badacl,
          uchar.fch.fch$v_nobackup?"NO":"",t_nobackup,
          uchar.fch.fch$v_contigb?"":"NO",t_contigb,
         uchar.fch.fch$v_nocharge?"NO":"",t_nocharge);
        printf("    %s%s,%s%s,%s%s,%s%s,%s%s, -\n",
          uchar.fch.fch$v_contig?"":"NO",t_contig,
          uchar.fch.fch$v_directory?"":"NO",t_directory,
          uchar.fch.fch$v_erase?"":"NO",t_erase,
          uchar.fch.fch$v_badblock?"":"NO",t_badblock,
          uchar.fch.fch$v_locked?"":"NO",t_locked);
        printf("    %s%s,%s%s,%s%s,%s%s,%s%s) -\n",
          uchar.fch.fch$v_markdel?"":"NO",t_markdel,
          uchar.fch.fch$v_spool?"":"NO",t_spool,
          uchar.fch.fch$v_readcheck?"":"NO",t_readcheck,
          uchar.fch.fch$v_writeback?"":"NO",t_writeback,
          uchar.fch.fch$v_writcheck?"":"NO",t_writcheck);
        printf("  /%s=%d",t_vfcsize,Fat.fat$b_vfcsize );
        printf("  /%s=%d",t_rsize,Fat.fat$w_rsize );
        printf("  /%s=%d -\n",t_maxrec,Fat.fat$w_maxrec );
        printf("  /%s=%d",t_efblk,
               (Fat.EFBLK.fat$w_efblkh<<16)|Fat.EFBLK.fat$w_efblkl);
        printf("  /%s=%d -\n",t_ffbyte,Fat.fat$w_ffbyte );
        printf("  /%s=%d -\n",t_hiblk,
               (Fat.HIBLK.fat$w_hiblkh<<16)|Fat.HIBLK.fat$w_hiblkl);
        printf("  /%s=%d -\n",t_defext,Fat.fat$w_defext );
        printf("  /%s=%d -\n",t_bktsize,Fat.fat$b_bktsize );
        printf("  /%s=%d -\n",t_gbc,Fat.fat$w_gbc );
        printf("  /%s=%d -\n",t_dirvers,Fat.fat$w_versions );
        printf("  /%s=%d -\n",t_versions,Fib.fib$w_verlimit );
        printf("  /%s=(%s%s,%s%s,%s%s,%s%s,%s%s,%s%s,%s%s) -\n",
          t_journal,
          jnl.fjn$v_journal_file?"":"NO",t_journal_file,
          jnl.fjn$v_aijnl?"":"NO",t_aijnl,
          jnl.fjn$v_atjnl?"":"NO",t_atjnl,
          jnl.fjn$v_bijnl?"":"NO",t_bijnl,
          jnl.fjn$v_rujnl?"":"NO",t_rujnl,
          jnl.fjn$v_never_ru?"":"NO",t_never_ru,
          jnl.fjn$v_only_ru?"":"NO",t_only_ru);
        if ((Cdate[0]==0) && (Cdate[1]==0))
          printf("  /NO%s -\n",t_credate);
        else
          printf("  /%s=\"%s\" -\n",t_credate,cvt_time(Cdate));
        if ((Rdate[0]==0) && (Rdate[1]==0))
          printf("  /NO%s",t_revdate);
        else
          printf("  /%s=\"%s\"",t_revdate,cvt_time(Rdate));
        printf("  /%s=%d -\n",t_revisions,revisions);
        if ((Edate[0]==0) && (Edate[1]==0))
          printf("  /NO%s -\n",t_expdate);
        else
          printf("  /%s=\"%s\" -\n",t_expdate,cvt_time(Edate));
        if ((Bdate[0]==0) && (Bdate[1]==0))
          printf("  /NO%s\n",t_bakdate);
        else
          printf("  /%s=\"%s\"\n",t_bakdate,cvt_time(Bdate));
      }; /* if (cli_present("OUTPUT") & 1) */
 
      if (cli_present(t_rtype) & 1) {
        status = cli_get_value(t_rtype,&retval);
        for (i=0;i<7;i++)
          if (strncmp(retval,ta_rtype[i],strlen(retval))==0)
            Fat.RTYPE.fat$v_rtype = i;
      };
      if (cli_present(t_fileorg) & 1) {
        status = cli_get_value(t_fileorg,&retval);
        for (i=0;i<4;i++)
          if (strncmp(retval,ta_fileorg[i],strlen(retval))==0)
            Fat.RTYPE.fat$v_fileorg = i;
      };
      if (cli_present(t_attr) & 1) {
        Fat.RATTRIB.fat$v_fortrancc =
            change(Fat.RATTRIB.fat$v_fortrancc,t_attr,t_fortrancc);
        Fat.RATTRIB.fat$v_impliedcc =
            change(Fat.RATTRIB.fat$v_impliedcc,t_attr,t_impliedcc);
        Fat.RATTRIB.fat$v_printcc   =
            change(Fat.RATTRIB.fat$v_printcc,t_attr,t_printcc);
        Fat.RATTRIB.fat$v_nospan    =
            antichange(Fat.RATTRIB.fat$v_nospan,t_attr,t_nospan);
      };
      if (cli_present(t_char) & 1) {
        uchar.fch.fch$v_badacl =
          change(uchar.fch.fch$v_badacl,t_char,t_badacl);
        uchar.fch.fch$v_nobackup =
          antichange(uchar.fch.fch$v_nobackup,t_char,t_nobackup);
        uchar.fch.fch$v_contigb =
          change(uchar.fch.fch$v_contigb,t_char,t_contigb);
        uchar.fch.fch$v_nocharge =
          antichange(uchar.fch.fch$v_nocharge,t_char,t_nocharge);
        uchar.fch.fch$v_contig =
          change(uchar.fch.fch$v_contig,t_char,t_contig);
        uchar.fch.fch$v_directory =
          change(uchar.fch.fch$v_directory,t_char,t_directory);
        uchar.fch.fch$v_erase =
          change(uchar.fch.fch$v_erase,t_char,t_erase);
        uchar.fch.fch$v_badblock =
          change(uchar.fch.fch$v_badblock,t_char,t_badblock);
        uchar.fch.fch$v_locked =
          change(uchar.fch.fch$v_locked,t_char,t_locked);
        uchar.fch.fch$v_markdel =
          change(uchar.fch.fch$v_markdel,t_char,t_markdel);
        uchar.fch.fch$v_spool = change(uchar.fch.fch$v_spool,t_char,t_spool);
        uchar.fch.fch$v_readcheck =
          change(uchar.fch.fch$v_readcheck,t_char,t_readcheck);
        uchar.fch.fch$v_writeback =
          change(uchar.fch.fch$v_writeback,t_char,t_writeback);
        uchar.fch.fch$v_writcheck =
          change(uchar.fch.fch$v_writcheck,t_char,t_writcheck);
      };
      if (cli_present(t_vfcsize) & 1) {
        status = cli_get_value(t_vfcsize,&retval);
        Fat.fat$b_vfcsize = atoi(retval);
      };
      if (cli_present(t_rsize) & 1) {
        status = cli_get_value(t_rsize,&retval);
        Fat.fat$w_rsize = atoi(retval);
      };
      if (cli_present(t_maxrec) & 1) {
        status = cli_get_value(t_maxrec,&retval);
        Fat.fat$w_maxrec = atoi(retval);
      };
      if (cli_present(t_efblk) & 1) {
        status = cli_get_value(t_efblk,&retval);
        Fat.EFBLK.fat$w_efblkh = atoi(retval)>>16;
        Fat.EFBLK.fat$w_efblkl = atoi(retval) & 0xffff;
      };
      if (cli_present(t_ffbyte) & 1) {
        status = cli_get_value(t_ffbyte,&retval);
        Fat.fat$w_ffbyte = atoi(retval);
      };
      if (cli_present(t_hiblk) & 1) {
        status = cli_get_value(t_hiblk,&retval);
        Fat.HIBLK.fat$w_hiblkh = atoi(retval)>>16;
        Fat.HIBLK.fat$w_hiblkl = atoi(retval) & 0xffff;
      };
      if (cli_present(t_defext) & 1) {
        status = cli_get_value(t_defext,&retval);
        Fat.fat$w_defext = atoi(retval);
      };
      if (cli_present(t_bktsize) & 1) {
        status = cli_get_value(t_bktsize,&retval);
        Fat.fat$b_bktsize = atoi(retval);
      };
      if (cli_present(t_gbc) & 1) {
        status = cli_get_value(t_gbc,&retval);
        Fat.fat$w_gbc = atoi(retval);
      };
      if (cli_present(t_dirvers) & 1) {
        status = cli_get_value(t_dirvers,&retval);
        Fat.fat$w_versions = atoi(retval);
      };
      if (cli_present(t_versions) & 1) {
        status = cli_get_value(t_versions,&retval);
        Fib.fib$w_verlimit = atoi(retval);
        if (Fib.fib$w_verlimit == 0)
          Fib.fib$w_verlimit = 32767;
      };
      /* if this value is illegal, don't try to set it! */
      if (Fib.fib$w_verlimit > 32767)
          Fib.fib$w_verlimit = 0;
      if (cli_present(t_journal) & 1) {
        jnl.fjn$v_journal_file =
            change(jnl.fjn$v_journal_file,t_journal,t_journal_file);
        jnl.fjn$v_aijnl = change(jnl.fjn$v_aijnl,t_journal,t_aijnl);
        jnl.fjn$v_atjnl = change(jnl.fjn$v_atjnl,t_journal,t_atjnl);
        jnl.fjn$v_bijnl = change(jnl.fjn$v_bijnl,t_journal,t_bijnl);
        jnl.fjn$v_rujnl = change(jnl.fjn$v_rujnl,t_journal,t_rujnl);
        jnl.fjn$v_never_ru = change(jnl.fjn$v_never_ru,t_journal,t_never_ru);
        jnl.fjn$v_only_ru = change(jnl.fjn$v_only_ru,t_journal,t_only_ru);
      };
      status = cli_present(t_credate);
      if (status & 1) {
        status = cli_get_value(t_credate,&retval);
        bintim(retval,Cdate);
      }
      else
        if ((status == CLI$_NEGATED) || (status == CLI$_LOCNEG)) {
          Cdate[0]=0;
          Cdate[1]=0;
        };
      status = cli_present(t_revdate);
      if (status & 1) {
        status = cli_get_value(t_revdate,&retval);
        bintim(retval,Rdate);
      }
      else
        if ((status == CLI$_NEGATED) || (status == CLI$_LOCNEG)) {
          Rdate[0]=0;
          Rdate[1]=0;
        };
      if (cli_present(t_revisions) & 1) {
        status = cli_get_value(t_revisions,&retval);
        revisions = atoi(retval);
      };
      status = cli_present(t_expdate);
      if (status & 1) {
        status = cli_get_value(t_expdate,&retval);
        bintim(retval,Edate);
      }
      else
        if ((status == CLI$_NEGATED) || (status == CLI$_LOCNEG)) {
          Edate[0]=0;
          Edate[1]=0;
        };
      status = cli_present(t_bakdate);
      if (status & 1) {
        status = cli_get_value(t_bakdate,&retval);
        bintim(retval,Bdate);
      }
      else
        if ((status == CLI$_NEGATED) || (status == CLI$_LOCNEG)) {
          Bdate[0]=0;
          Bdate[1]=0;
        };
 
      status = cli_present(t_owner);
      if (status & 1) {
        status = cli_get_value(t_owner,&string.dsc$a_pointer);
        if (!(status & 1)) return(status);
        string.dsc$w_length = strlen (string.dsc$a_pointer);
        status = parse_uic(&string, &uic);
        if (!(status & 1))
          printf("UIC %.*s is not valid\n",
                 string.dsc$w_length,string.dsc$a_pointer);
      }
 
      status = cli_present(t_protection);
      if (status & 1) {
          prot.bits.system = parse_protection(t_protection,"SYSTEM",
                                              prot.bits.system);
          prot.bits.owner  = parse_protection(t_protection,"OWNER",
                                              prot.bits.owner);
          prot.bits.group  = parse_protection(t_protection,"GROUP",
                                              prot.bits.group);
          prot.bits.world  = parse_protection(t_protection,"WORLD",
                                              prot.bits.world);
      }
 
      /* note, part of the FIB was cleared by earlier QIOW, so reset it */
      Fib.fib$r_acctl_overlay.fib$l_acctl = FIB$M_NORECORD;
      for (i=0;i<3;i++)
        Fib.fib$r_fid_overlay.fib$w_fid[i]=Nam.nam$w_fid[i];
      for (i=0;i<3;i++)
        Fib.fib$r_did_overlay.fib$w_did[i]=Nam.nam$w_did[i];
 
      /* Use the IO$_MODIFY function to change info about the file */
      /* Note, used this way, the file is not opened, however this would */
      /* normally cause the expiration and revision dates to be modified. */
      /* Using FIB$M_NORECORD prohibits this from happening. */
      status = sys$qiow(0,DevChan,IO$_MODIFY,&iosb,0,0,
                        &FibDesc,&FileName,0,0,&Atr,0);
      if (!(status & 1)) return(status);
 
      status = iosb[0];
      if (!(status & 1)) return(status);
 
      status = sys$dassgn(DevChan);
      if (!(status & 1)) return(status);
 
      /* look for next file, if none, no big deal.. */
      status = sys$search(&Fab);
      }
  }
}
 
long int cli_present(s)
  char *s;
{
  static struct dsc$descriptor s_desc = {0,DSC$K_DTYPE_T,DSC$K_CLASS_S,0};
 
  s_desc.dsc$w_length = strlen(s);
  s_desc.dsc$a_pointer = s;
  return(cli$present(&s_desc));
}
 
long int cli_get_value(s1,s2)
  char *s1,**s2;
{
  static struct dsc$descriptor s1_desc={0,DSC$K_DTYPE_T,DSC$K_CLASS_S,0};
  static struct dsc$descriptor s2_desc={0,DSC$K_DTYPE_T,DSC$K_CLASS_D,0};
  static char null = '\0';
  static struct dsc$descriptor null_desc =
         {1,DSC$K_DTYPE_T,DSC$K_CLASS_S,&null};
  long int status;
 
  s1_desc.dsc$w_length = strlen(s1);
  s1_desc.dsc$a_pointer = s1;
 
  status = cli$get_value(&s1_desc,&s2_desc);
 
  if (status & 1)
    {
    str$append(&s2_desc,&null_desc);
    *s2 = s2_desc.dsc$a_pointer;
    }
  else
    *s2 = 0;
  return(status);
}
 
char *cvt_time(date)
  long int date[2];
{
  static char str[27];
  static struct dsc$descriptor date_str={26,DSC$K_DTYPE_T,DSC$K_CLASS_S,&str};
 
  if ((date[0]==0) && (date[1]==0))
    return("none");
 
  sys$asctim(0,&date_str,date,0);
  str[26]='\0';
 
  return(&str);
}
 
bintim(time,binval)
  char *time;
  long int binval[2];
{
  static struct dsc$descriptor date_str={0,DSC$K_DTYPE_T,DSC$K_CLASS_S,0};
  date_str.dsc$w_length = strlen(time);
  date_str.dsc$a_pointer = time;
 
  sys$bintim(&date_str,binval);
}
 
unsigned int change(orig,s1,s2)
  unsigned int orig;
  char *s1,*s2;
{
  static struct dsc$descriptor s1_desc={0,DSC$K_DTYPE_T,DSC$K_CLASS_S,0};
  static struct dsc$descriptor s2_desc={0,DSC$K_DTYPE_T,DSC$K_CLASS_S,0};
  static struct dsc$descriptor s3_desc={0,DSC$K_DTYPE_T,DSC$K_CLASS_D,0};
  static char point = '.';
  static struct dsc$descriptor pt_desc={1,DSC$K_DTYPE_T,DSC$K_CLASS_S,&point};
  long int status;
  unsigned int i;
 
  s1_desc.dsc$w_length = strlen(s1);
  s1_desc.dsc$a_pointer = s1;
  s2_desc.dsc$w_length = strlen(s2);
  s2_desc.dsc$a_pointer = s2;
 
  /* s3 = s1 + "." + s2 */
  str$copy_dx(&s3_desc,&s1_desc);
  str$append(&s3_desc,&pt_desc);
  str$append(&s3_desc,&s2_desc);
 
  status = cli$present(&s3_desc);
  i=orig;
  if (status & 1)
    i=1;
  if ((status == CLI$_NEGATED) || (status == CLI$_LOCNEG))
    i=0;
  return(i);
}
 
unsigned int antichange(orig,s1,s2)
  unsigned int orig;
  char *s1,*s2;
{
  static struct dsc$descriptor s1_desc={0,DSC$K_DTYPE_T,DSC$K_CLASS_S,0};
  static struct dsc$descriptor s2_desc={0,DSC$K_DTYPE_T,DSC$K_CLASS_S,0};
  static struct dsc$descriptor s3_desc={0,DSC$K_DTYPE_T,DSC$K_CLASS_D,0};
  static char point = '.';
  static struct dsc$descriptor pt_desc={1,DSC$K_DTYPE_T,DSC$K_CLASS_S,&point};
  long int status;
  unsigned int i;
 
  s1_desc.dsc$w_length = strlen(s1);
  s1_desc.dsc$a_pointer = s1;
  s2_desc.dsc$w_length = strlen(s2);
  s2_desc.dsc$a_pointer = s2;
 
  /* s3 = s1 + "." + s2 */
  str$copy_dx(&s3_desc,&s1_desc);
  str$append(&s3_desc,&pt_desc);
  str$append(&s3_desc,&s2_desc);
 
  status = cli$present(&s3_desc);
  i=orig;
  if (status & 1)
    i=0;
  if ((status == CLI$_NEGATED) || (status == CLI$_LOCNEG))
    i=1;
  return(i);
}
 
int parse_protection(char *s1, char *s2, unsigned int orig)
{
  static struct dsc$descriptor s1_desc={0,DSC$K_DTYPE_T,DSC$K_CLASS_S,0};
  static struct dsc$descriptor s2_desc={0,DSC$K_DTYPE_T,DSC$K_CLASS_S,0};
  static struct dsc$descriptor s3_desc={0,DSC$K_DTYPE_T,DSC$K_CLASS_D,0};
  static char point = '.';
  static struct dsc$descriptor pt_desc={1,DSC$K_DTYPE_T,DSC$K_CLASS_S,&point};
  long int status, j;
  char *c;
  unsigned int i = orig;
 
  s1_desc.dsc$w_length = strlen(s1);
  s1_desc.dsc$a_pointer = s1;
  s2_desc.dsc$w_length = strlen(s2);
  s2_desc.dsc$a_pointer = s2;
 
  /* s3 = s1 + "." + s2 */
  str$copy_dx(&s3_desc,&s1_desc);
  str$append(&s3_desc,&pt_desc);
  str$append(&s3_desc,&s2_desc);
 
  status = cli$present(&s3_desc);
  if (status & 1)
  {
    i = 0x0f;
    status = cli$get_value(&s3_desc,&s3_desc);
    if (status & 1)
    {
      /* loop through checking for R, W, E, D */
      c = s3_desc.dsc$a_pointer;
      for (j=0;j<s3_desc.dsc$w_length;j++)
      {
        switch (*c)
        {
          case 'R' : i &= ~1; break;
          case 'W' : i &= ~2; break;
          case 'E' : i &= ~4; break;
          case 'D' : i &= ~8; break;
          default:
            printf("Illegal protection specified '%c'\n",*c);
            break;
        };
        ++c;
      };
    };
  };
  return(i);
}
