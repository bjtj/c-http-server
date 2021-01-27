#include <osl/osl.h>
#include <osl/str.h>
#include <assert.h>
#include "header.h"

static void test_parse_header_firstline1(void)
{
    chttpserver_header_firstline_t * firstline = chttpserver_header_firstline_from_str("GET / HTTP/1.1\r\n");
    assert(strcmp(firstline->part1, "GET") == 0);
    assert(strcmp(firstline->part2, "/") == 0);
    assert(strcmp(firstline->part3, "HTTP/1.1") == 0);
    chttpserver_header_firstline_free(firstline);
}

static void test_parse_header_firstline2(void)
{
    chttpserver_header_firstline_t * firstline = chttpserver_header_firstline_from_str("GET / HTTP/1.1");
    assert(strcmp(firstline->part1, "GET") == 0);
    assert(strcmp(firstline->part2, "/") == 0);
    assert(strcmp(firstline->part3, "HTTP/1.1") == 0);
    chttpserver_header_firstline_free(firstline);
}

static void test_parse_header_firstline3(void)
{
    chttpserver_header_firstline_t * firstline = chttpserver_header_firstline_from_str("GET \t/\t\tHTTP/1.1");
    assert(strcmp(firstline->part1, "GET") == 0);
    assert(strcmp(firstline->part2, "/") == 0);
    assert(strcmp(firstline->part3, "HTTP/1.1") == 0);
    chttpserver_header_firstline_free(firstline);
}

static void test_parse_fields1(void)
{
    osl_list_t * fields = chttpserver_header_fields_from_str("\r\n\r\n");
    assert(fields == NULL);
    osl_list_free(fields, (osl_free_cb)chttpserver_header_field_free);
}

static void test_parse_fields2(void)
{
    osl_list_t * fields = chttpserver_header_fields_from_str("name: value\r\n\r\n");
    assert(fields != NULL);
    /* TODO: get field test */
    osl_list_free(fields, (osl_free_cb)chttpserver_header_field_free);
}

static void test_parse_header1(void)
{
    chttpserver_header_t * header = chttpserver_header_from_str("GET / HTTP/1.1\r\n\r\n");
    assert(strcmp(header->firstline->part1, "GET") == 0);
    assert(strcmp(header->firstline->part2, "/") == 0);
    assert(strcmp(header->firstline->part3, "HTTP/1.1") == 0);
    chttpserver_header_free(header);
}


static void test_parse_header2(void)
{
    chttpserver_header_t * header = chttpserver_header_from_str("GET / HTTP/1.1\r\nConnection: keep-alive\r\n\r\n");
    assert(strcmp(header->firstline->part1, "GET") == 0);
    assert(strcmp(header->firstline->part2, "/") == 0);
    assert(strcmp(header->firstline->part3, "HTTP/1.1") == 0);
    assert(osl_string_equals(chttpserver_header_get_field_value(header, "Connection"), "keep-alive"));
    chttpserver_header_free(header);
}

static void test_header_field(void)
{
    const char * connection;
    chttpserver_header_t * header = chttpserver_header_init(chttpserver_header_new(), CHTTPSERVER_REQUEST_HEADER, CHTTPSERVER_HTTP_1_1);
    assert(osl_string_equals(header->firstline->part3, "HTTP/1.1"));
    assert(header->fields == NULL);
    assert(chttpserver_header_get_field(header, "Connection") == NULL);
    chttpserver_header_set_field_value(header, "Connection", "keep-alive");
    assert(header->fields != NULL);
    assert(chttpserver_header_get_field(header, "Connection") != NULL);
    assert(osl_string_equals(chttpserver_header_get_field(header, "Connection")->name, "Connection"));
    assert(chttpserver_header_get_field(header, "Connection")->values != NULL);
    connection = chttpserver_header_get_field_value(header, "Connection");
    assert(connection != NULL);
    assert(osl_string_equals(connection, "keep-alive"));
    chttpserver_header_free(header);
}


int main(void)
{
    test_parse_header_firstline1();
    test_parse_header_firstline2();
    test_parse_header_firstline3();
    test_parse_fields1();
    test_parse_fields2();
    test_parse_header1();
    test_parse_header2();
    test_header_field();
    return 0;
}
