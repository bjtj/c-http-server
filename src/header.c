#include <osl/pathname.h>
#include <osl/str.h>
#include <osl/string_buffer.h>
#include "header.h"


chttpserver_header_t * chttpserver_header_new(void)
{
    chttpserver_header_t * header = (chttpserver_header_t *)malloc(sizeof(chttpserver_header_t));
    if (header == NULL) {
	/* TODO: exception */
	return NULL;
    }
    memset(header, 0, sizeof(chttpserver_header_t));
    return header;
}

chttpserver_header_t * chttpserver_header_init(chttpserver_header_t * header, chttpserver_header_firstline_t * firstline)
{
    if (firstline == NULL) {
	header->firstline = chttpserver_header_firstline_init(chttpserver_header_firstline_new());
    } else {
	header->firstline = firstline;
    }
    
    return header;
}

void chttpserver_header_free(chttpserver_header_t * header)
{
    if (header == NULL) {
	return;
    }
    chttpserver_header_firstline_free(header->firstline);
    osl_list_free(header->fields, (osl_free_cb)chttpserver_header_field_free);
    osl_safe_free(header);
}

chttpserver_header_field_t * chttpserver_header_get_field(chttpserver_header_t * header, const char * name)
{
    return (chttpserver_header_field_t *)osl_list_find(header->fields, (osl_compare_cb)chttpserver_header_field_compare_name, (void*)name);
}

char * chttpserver_header_get_field_value(chttpserver_header_t * header, const char * name)
{
    chttpserver_header_field_t * field = chttpserver_header_get_field(header, name);
    return field ? (field->values ? (char*)field->values->data : NULL) : NULL;
}

osl_list_t * chttpserver_header_get_field_values(chttpserver_header_t * header, const char * name)
{
    chttpserver_header_field_t * field = chttpserver_header_get_field(header, name);
    return field ? field->values : NULL;
}

void chttpserver_header_set_field_value(chttpserver_header_t * header, const char * name, const char * value)
{
    chttpserver_header_field_t * field = chttpserver_header_get_field(header, name);
    if (field == NULL) {
	header->fields = osl_list_append(header->fields, chttpserver_header_field_init_with_name_value(chttpserver_header_field_new(), name, value));
    } else {
	osl_list_free(field->values, (osl_free_cb)chttpserver_header_field_free);
	field->values = osl_list_append(field->values, osl_strdup(value));
    }
}

void chttpserver_header_set_field_values(chttpserver_header_t * header, const char * name, osl_list_t * values)
{
    chttpserver_header_field_t * field = chttpserver_header_get_field(header, name);
    if (field == NULL) {
	header->fields = osl_list_append(header->fields, chttpserver_header_field_init_with_name_values(chttpserver_header_field_new(), name, values));
    } else {
	osl_list_free(field->values, (osl_free_cb)chttpserver_header_field_free);
	field->values = values;
    }
}

void chttpserver_header_remove_field(chttpserver_header_t * header, const char * name)
{
    header->fields = osl_list_remove_if(header->fields, (osl_compare_cb)chttpserver_header_field_compare_name, (void*)name, (osl_free_cb)chttpserver_header_field_free);
}

    
chttpserver_header_firstline_t * chttpserver_header_firstline_new(void)
{
    chttpserver_header_firstline_t * firstline = malloc(sizeof(chttpserver_header_firstline_t));
    if (firstline == NULL) {
	return NULL;
    }
    memset(firstline, 0, sizeof(chttpserver_header_firstline_t));
    return firstline;
}

chttpserver_header_firstline_t * chttpserver_header_firstline_init(chttpserver_header_firstline_t * firstline)
{
    return firstline;
}

void chttpserver_header_firstline_free(chttpserver_header_firstline_t * firstline)
{
    if (firstline == NULL) {
	return;
    }
    osl_safe_free(firstline->line);
    osl_safe_free(firstline->part1);
    osl_safe_free(firstline->part2);
    osl_safe_free(firstline->part3);
    osl_safe_free(firstline);
}

osl_bool chttpserver_header_firstline_is_empty(chttpserver_header_firstline_t * firstline)
{
    if (firstline == NULL) {
	return osl_true;
    }

    if (firstline->part1 == NULL && firstline->part2 == NULL && firstline->part3 == NULL) {
	return osl_true;
    }

    return osl_false;
}

void chttpserver_header_firstline_set_part1(chttpserver_header_firstline_t * firstline, const char * part1)
{
    osl_safe_free(firstline->part1);
    firstline->part1 = osl_strdup(part1);
}

void chttpserver_header_firstline_set_part2(chttpserver_header_firstline_t * firstline, const char * part2)
{
    osl_safe_free(firstline->part2);
    firstline->part2 = osl_strdup(part2);
}

void chttpserver_header_firstline_set_part3(chttpserver_header_firstline_t * firstline, const char * part3)
{
    osl_safe_free(firstline->part3);
    firstline->part3 = osl_strdup(part3);
}


chttpserver_header_field_t * chttpserver_header_field_new(void)
{
    chttpserver_header_field_t * field = (chttpserver_header_field_t *)malloc(sizeof(chttpserver_header_field_t));
    if (field == NULL) {
	/* TODO: exception */
	perror("Allocating header field");
	return NULL;
    }
    memset(field, 0, sizeof(chttpserver_header_field_t));
    return field;
}

chttpserver_header_field_t * chttpserver_header_field_init(chttpserver_header_field_t * field)
{
    return field;
}

chttpserver_header_field_t * chttpserver_header_field_init_with_name_value(chttpserver_header_field_t * field, const char * name, const char * value)
{
    field->name = osl_strdup(name);
    field->values = osl_list_append(field->values, osl_strdup(value));
    return field;
}

chttpserver_header_field_t * chttpserver_header_field_init_with_name_values(chttpserver_header_field_t * field, const char * name, osl_list_t * values)
{
    field->name = osl_strdup(name);
    field->values = values;
    return field;
}

void chttpserver_header_field_free(chttpserver_header_field_t * field)
{
    if (field == NULL) {
	return;
    }
    osl_safe_free(field->name);
    osl_list_free(field->values, osl_safe_free);
    osl_safe_free(field);
}

osl_bool chttpserver_header_field_compare_name(chttpserver_header_field_t * field, const char * name)
{
    return osl_string_equals_ignorecase(field->name, name);
}

chttpserver_header_t * chttpserver_header_from_str(const char * header_string)
{
    const char * fields_string;
    chttpserver_header_t * header;
    header = chttpserver_header_init(chttpserver_header_new(), chttpserver_header_firstline_from_str(header_string));
    fields_string = osl_string_find(header_string, "\r\n") + 2;
    header->fields = chttpserver_header_fields_from_str(fields_string);
    return header;
}
    
chttpserver_header_firstline_t * chttpserver_header_firstline_from_str(const char * header)
{
    chttpserver_header_firstline_t * firstline = NULL;
    const char * space = NULL;
    const char * token = NULL;
    firstline = chttpserver_header_firstline_init(chttpserver_header_firstline_new());
    if (firstline == NULL) {
	/* TODO: exception */
	return NULL;
    }
    token = osl_string_find(header, "\r\n");
    if (token == NULL) {
	firstline->line = osl_strdup(header);
    } else {
	firstline->line = osl_strndup(header, token - header);
    }

    token = firstline->line;
    space = osl_string_find_first_of(token, " \t");
    firstline->part1 = osl_strndup(token, space - token);
    
    token = osl_string_find_first_not_of(space, " \t");
    space = osl_string_find_first_of(token, " \t");
    firstline->part2 = osl_strndup(token, space - token);

    token = osl_string_find_first_not_of(space, " \t");
    firstline->part3 = osl_strdup(token);

    return firstline;
}

osl_list_t * chttpserver_header_fields_from_str(const char * fields_string)
{
    osl_list_t * fields = NULL;
    const char * ptr = fields_string;
    const char * end = osl_string_find(ptr, "\r\n");
    while (end && ptr != end)
    {
	chttpserver_header_field_t * field = chttpserver_header_field_from_str(ptr);
	fields = osl_list_append(fields, field);
	ptr = end + 2;
	end = osl_string_find(ptr, "\r\n");
    }
    return fields;
}

chttpserver_header_field_t * chttpserver_header_field_from_str(const char * line)
{
    chttpserver_header_field_t * result = NULL;
    const char * end = osl_string_find(line, "\r\n");
    const char * delimeter = osl_string_find(line, ":");
    char * name = NULL;
    char * value = NULL;
    if (end == NULL) {
	end = osl_string_end_ptr(line);
    }
    if (delimeter == NULL) {
	name = osl_strndup(line, end - line);
    } else {
	name = osl_strndup(line, delimeter - line);
	value = osl_strndup(delimeter + 1, end - delimeter - 1);
    }
    result = chttpserver_header_field_init_with_name_value(chttpserver_header_field_new(), name, value);
    osl_safe_free(name);
    osl_safe_free(value);
    return result;
}

char * chttpserver_header_to_str(chttpserver_header_t * header)
{
    char firstline[256] = {0,};
    char fieldline[256] = {0,};
    osl_string_buffer_t * sb;

    if (header == NULL) {
	return NULL;
    }

    if (chttpserver_header_firstline_is_empty(header->firstline)) {
	return NULL;
    }
    
    sb = osl_string_buffer_init(osl_string_buffer_new());

    snprintf(firstline, sizeof(firstline), "%s %s %s\r\n", header->firstline->part1, header->firstline->part2, header->firstline->part3);
    osl_string_buffer_append(sb, firstline);

    osl_list_t * ptr = header->fields;
    for (; ptr; ptr = ptr->next) {
	chttpserver_header_field_t * field = (chttpserver_header_field_t *)ptr->data;
	osl_list_t * value_ptr = field->values;
	for (; value_ptr; value_ptr = value_ptr->next) {
	    snprintf(fieldline, sizeof(fieldline), "%s: %s\r\n", field->name, (char*)value_ptr->data);
	    osl_string_buffer_append(sb, fieldline);
	}
    }
    osl_string_buffer_append(sb, "\r\n");
    return osl_string_buffer_to_str_and_free(sb);
}
