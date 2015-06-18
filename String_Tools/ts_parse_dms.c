/* DEC/CMS REPLACEMENT HISTORY, Element TS_PARSE_DMS.C */
/* *3     8-JUL-1991 11:36:13 CHEW "(SPR 7344) Check for negative minutes and seconds, if degree = 0." */
/* *2    11-JUN-1991 12:48:33 JULIAN "(SPR 0) new element" */
/* *1    11-JUN-1991 12:47:23 JULIAN "parse dms format" */
/* DEC/CMS REPLACEMENT HISTORY, Element TS_PARSE_DMS.C */
#include "esi_gl_defs.h"
#include "esi_c_lib.h"
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#include "esi_md_msg.h"
#include "esi_ct.h"

#if USE_PROTOTYPES
publicdef INT ts_parse_dms(CHAR *str, LATLONG *pLatitude, LATLONG *pLongitude)
#else
publicdef INT ts_parse_dms(str, pLatitude, pLongitude)
CHAR *str;
LATLONG *pLatitude;
LATLONG *pLongitude;
#endif
    {
    INT status = SUCCESS;
    INT len, word_cnt = 0;
    INT degrees, minutes;
    DOUBLE seconds;
    CHAR tmp[10];
    CHAR *ptr, *has_min, *has_sec;
    CHAR str_left[50], str_right[50], token[50], *s;
    
    if (IS_EMPTY_STRING(str))
        return MD_BAD_DMS;
    
    if ((ptr = strchr(str, ',')) == 0)
        return MD_BAD_DMS;
    
    strncpy(str_left, str, ptr - str);
    str_left[ptr - str] = '\0';
    strcpy(str_right, ptr + 1);
    
    /* Parse out the left side                                                      */
    
    ts_snowhite(str_left);
    has_min = strchr(str_left, '\'');
    has_sec = strchr(str_left, '\"');
    
    if (has_sec && !has_min)
        return MD_BAD_DMS;
    
    len = strlen(str_left);
    if (has_min)
        ts_str_convert(str_left, len, '\'', ' ');
    if (has_sec)
        ts_str_convert(str_left, len, '\"', ' ');
    
    degrees = minutes = seconds = 0;
    tmp[0] = '\0';
    
    s = ts_next_word(str_left, token);
    word_cnt++;
    if (ts_isfloat(token))
        {
        degrees = atoi(token);
        }
    else
        {
        return MD_BAD_DMS;
        }
    while (s)
        {
        s = ts_next_word(s, token);
        word_cnt++;
        switch (word_cnt)
            {
        case 2:
            if (has_min)
                {
                if (ts_isfloat(token))
                    {
                    minutes = atoi(token);
                    }
                else
                    {
                    return MD_BAD_DMS;
                    }
                }
            else
                {
                strncpy(tmp, token, 1);
                tmp[1] = '\0';
                }
            break;
        case 3:
            if (has_sec)
                {
                if (ts_isfloat(token))
                    {
                    seconds = atof(token);
                    }
                else
                    {
                    return MD_BAD_DMS;
                    }
                }
            else
                {
                strncpy(tmp, token, 1);
                tmp[1] = '\0';
                }
            break;
        case 4:
            strncpy(tmp, token, 1);
            tmp[1] = '\0';
            break;
        default:
            return MD_BAD_DMS;
            break;
            }
        }
    if (word_cnt < 2)
        return MD_BAD_DMS;
    
    if (IS_EMPTY_STRING(tmp))
        return MD_BAD_DMS;
    
    ts_sto_upper(tmp);
    if (ARE_SAME(tmp, "W"))
    {
        if (degrees != 0)
            degrees *= -1;
        else if (minutes != 0)
            minutes *= -1;
        else 
            seconds *= -1;
    }
    
    pLongitude->dms.degrees = degrees;
    pLongitude->dms.minutes = minutes;
    pLongitude->dms.seconds = seconds;
    
    /* Parse out the right side                                                     */
    
    ts_snowhite(str_right);
    has_min = strchr(str_right, '\'');
    has_sec = strchr(str_right, '\"');
    
    if (has_sec && !has_min)
        return MD_BAD_DMS;
    
    len = strlen(str_right);
    if (has_min)
        ts_str_convert(str_right, len, '\'', ' ');
    if (has_sec)
        ts_str_convert(str_right, len, '\"', ' ');
    
    degrees = minutes = seconds = word_cnt = 0;
    tmp[0] = '\0';
    
    s = ts_next_word(str_right, token);
    word_cnt++;
    if (ts_isfloat(token))
        {
        degrees = atoi(token);
        }
    else
        {
        return MD_BAD_DMS;
        }
    while (s)
        {
        s = ts_next_word(s, token);
        word_cnt++;
        switch (word_cnt)
            {
        case 2:
            if (has_min)
                {
                if (ts_isfloat(token))
                    {
                    minutes = atoi(token);
                    }
                else
                    {
                    return MD_BAD_DMS;
                    }
                }
            else
                {
                strncpy(tmp, token, 1);
                tmp[1] = '\0';
                }
            break;
        case 3:
            if (has_sec)
                {
                if (ts_isfloat(token))
                    {
                    seconds = atof(token);
                    }
                else
                    {
                    return MD_BAD_DMS;
                    }
                }
            else
                {
                strncpy(tmp, token, 1);
                tmp[1] = '\0';
                }
            break;
        case 4:
            strncpy(tmp, token, 1);
            tmp[1] = '\0';
            break;
        default:
            return MD_BAD_DMS;
            break;
            }
        }
    if (word_cnt < 2)
        return MD_BAD_DMS;
    
    if (IS_EMPTY_STRING(tmp))
        return MD_BAD_DMS;
    
    ts_sto_upper(tmp);
    if (ARE_SAME(tmp, "S"))
    {
        if (degrees != 0)
            degrees *= -1;
        else if (minutes != 0)
            minutes *= -1;
        else 
            seconds *= -1;
    }
    
    pLatitude->dms.degrees = degrees;
    pLatitude->dms.minutes = minutes;
    pLatitude->dms.seconds = seconds;
    
    return SUCCESS;
    }
