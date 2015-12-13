#ifndef JSAPI_HPP
#define JSAPI_HPP

#include "duktape.h"

duk_ret_t btx_flip(duk_context *ctx);
duk_ret_t btx_triangle(duk_context *ctx);
duk_ret_t btx_triangle_color(duk_context *ctx);
duk_ret_t btx_tricolor_triangle(duk_context *ctx);
duk_ret_t btx_rect(duk_context *ctx);
duk_ret_t btx_quad(duk_context *ctx);
duk_ret_t btx_set_clear_color(duk_context *ctx);
duk_ret_t btx_create_window(duk_context *ctx);
duk_ret_t btx_poll_event(duk_context *ctx);
duk_ret_t btx_sanity(duk_context *ctx);
duk_ret_t btx_get_ticks(duk_context *ctx);
duk_ret_t btx_delay(duk_context *ctx);
duk_ret_t btx_include(duk_context *ctx);
duk_ret_t btx_randrange(duk_context *ctx);
duk_ret_t btx_uniform(duk_context *ctx);

#endif
