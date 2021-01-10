#ifndef __HTTP_HEADER_H__
#define __HTTP_HEADER_H__

#include <osl/osl.h>
#include <osl/list.h>

typedef struct _chttpserver_header_firstline_t {
    char * line;
    char * part1;
    char * part2;
    char * part3;
} chttpserver_header_firstline_t;

typedef struct _chttpserver_header_field_t {
    char * name;
    osl_list_t * values;
    struct _chttpserver_header_field_t * next;
} chttpserver_header_field_t;

typedef struct _chttpserver_header_t {
    chttpserver_header_firstline_t * firstline;
    osl_list_t * fields;
} chttpserver_header_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT chttpserver_header_t * chttpserver_header_new(void);
    extern OSL_EXPORT void chttpserver_header_free(chttpserver_header_t * header);
    extern OSL_EXPORT chttpserver_header_field_t * chttpserver_header_get_field(chttpserver_header_t * header, const char * name);
    extern OSL_EXPORT char * chttpserver_header_get_field_value(chttpserver_header_t * header, const char * name);
    extern OSL_EXPORT osl_list_t * chttpserver_header_get_field_values(chttpserver_header_t * header, const char * name);
    extern OSL_EXPORT void chttpserver_header_set_field_value(chttpserver_header_t * header, const char * name, const char * value);
    extern OSL_EXPORT void chttpserver_header_set_field_values(chttpserver_header_t * header, const char * name, osl_list_t * values);
    extern OSL_EXPORT void chttpserver_header_remove_field(chttpserver_header_t * header, const char * name);
    
    extern OSL_EXPORT chttpserver_header_firstline_t * chttpserver_header_firstline_new(void);
    extern OSL_EXPORT void chttpserver_header_firstline_free(chttpserver_header_firstline_t * firstline);
    extern OSL_EXPORT void chttpserver_header_firstline_set_part1(chttpserver_header_firstline_t * firstline, const char * part1);
    extern OSL_EXPORT void chttpserver_header_firstline_set_part2(chttpserver_header_firstline_t * firstline, const char * part2);
    extern OSL_EXPORT void chttpserver_header_firstline_set_part3(chttpserver_header_firstline_t * firstline, const char * part3);

    extern OSL_EXPORT chttpserver_header_field_t * chttpserver_header_field_new(void);
    extern OSL_EXPORT chttpserver_header_field_t * chttpserver_header_field_new_with_name_value(const char * name, const char * value);
    extern OSL_EXPORT chttpserver_header_field_t * chttpserver_header_field_new_with_name_values(const char * name, osl_list_t * values);
    extern OSL_EXPORT void chttpserver_header_field_free(chttpserver_header_field_t * field);
    extern OSL_EXPORT osl_bool chttpserver_header_field_compare_name(chttpserver_header_field_t * field, const char * name);

    extern OSL_EXPORT chttpserver_header_t * chttpserver_header_read(const char * header);
    extern OSL_EXPORT chttpserver_header_firstline_t * chttpserver_header_firstline_read(const char * header);
    extern OSL_EXPORT osl_list_t * chttpserver_header_fields_read(const char * fields);
    extern OSL_EXPORT chttpserver_header_field_t * chttpserver_header_field_read(const char * line);

#ifdef __cplusplus
}
#endif

#endif /* __HTTP_HEADER_H__ */
