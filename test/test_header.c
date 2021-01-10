#include <osl/osl.h>
#include <assert.h>
#include "header.h"

void test_parse_header_firstline1(void)
{
    chttpserver_header_firstline_t * firstline = chttpserver_header_firstline_read("GET / HTTP/1.1\r\n");
    assert(strcmp(firstline->part1, "GET") == 0);
    assert(strcmp(firstline->part2, "/") == 0);
    assert(strcmp(firstline->part3, "HTTP/1.1") == 0);
    chttpserver_header_firstline_free(firstline);
}

void test_parse_header_firstline2(void)
{
    chttpserver_header_firstline_t * firstline = chttpserver_header_firstline_read("GET / HTTP/1.1");
    assert(strcmp(firstline->part1, "GET") == 0);
    assert(strcmp(firstline->part2, "/") == 0);
    assert(strcmp(firstline->part3, "HTTP/1.1") == 0);
    chttpserver_header_firstline_free(firstline);
}

void test_parse_header_firstline3(void)
{
    chttpserver_header_firstline_t * firstline = chttpserver_header_firstline_read("GET \t/\t\tHTTP/1.1");
    assert(strcmp(firstline->part1, "GET") == 0);
    assert(strcmp(firstline->part2, "/") == 0);
    assert(strcmp(firstline->part3, "HTTP/1.1") == 0);
    chttpserver_header_firstline_free(firstline);
}

void test_parse_fields1(void)
{
    osl_list_t * fields = chttpserver_header_fields_read("\r\n\r\n");
    assert(fields == NULL);
    osl_list_free(fields, (osl_free_cb)chttpserver_header_field_free);
}

void test_parse_fields2(void)
{
    osl_list_t * fields = chttpserver_header_fields_read("name: value\r\n\r\n");
    assert(fields != NULL);
    /* TODO: get field test */
    osl_list_free(fields, (osl_free_cb)chttpserver_header_field_free);
}

void test_parse_header1(void)
{
    chttpserver_header_t * header = chttpserver_header_read("GET / HTTP/1.1\r\n\r\n");
    assert(strcmp(header->firstline->part1, "GET") == 0);
    assert(strcmp(header->firstline->part2, "/") == 0);
    assert(strcmp(header->firstline->part3, "HTTP/1.1") == 0);
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
    return 0;
}
