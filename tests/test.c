/*
 * Copyright (c) 2014 Christopher M. Baker
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include <jsun.h>
#include <assert.h>

int
main(void) {
    jsun_t jsun[32];

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, '_') == JSUN_ERROR);

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, '\f') == JSUN_NONE);
    assert(jsun_step(jsun, '\n') == JSUN_NONE);
    assert(jsun_step(jsun, '\r') == JSUN_NONE);
    assert(jsun_step(jsun, '\t') == JSUN_NONE);
    assert(jsun_step(jsun, '\v') == JSUN_NONE);
    assert(jsun_step(jsun, ' ')  == JSUN_NONE);

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, 't') == JSUN_NONE);
    assert(jsun_step(jsun, 'r') == JSUN_NONE);
    assert(jsun_step(jsun, 'u') == JSUN_NONE);
    assert(jsun_step(jsun, 'e') == JSUN_TRUE);
    assert(jsun_content_length(jsun) == 0);

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, 't') == JSUN_NONE);
    assert(jsun_step(jsun, 'r') == JSUN_NONE);
    assert(jsun_step(jsun, 'u') == JSUN_NONE);
    assert(jsun_step(jsun, '_') == JSUN_ERROR);
    assert(jsun_content_length(jsun) == 0);

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, 'f') == JSUN_NONE);
    assert(jsun_step(jsun, 'a') == JSUN_NONE);
    assert(jsun_step(jsun, 'l') == JSUN_NONE);
    assert(jsun_step(jsun, 's') == JSUN_NONE);
    assert(jsun_step(jsun, 'e') == JSUN_FALSE);
    assert(jsun_content_length(jsun) == 0);

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, 'f') == JSUN_NONE);
    assert(jsun_step(jsun, 'a') == JSUN_NONE);
    assert(jsun_step(jsun, 'l') == JSUN_NONE);
    assert(jsun_step(jsun, 's') == JSUN_NONE);
    assert(jsun_step(jsun, '_') == JSUN_ERROR);
    assert(jsun_content_length(jsun) == 0);

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, 'n') == JSUN_NONE);
    assert(jsun_step(jsun, 'u') == JSUN_NONE);
    assert(jsun_step(jsun, 'l') == JSUN_NONE);
    assert(jsun_step(jsun, 'l') == JSUN_NULL);
    assert(jsun_content_length(jsun) == 0);

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, 'n') == JSUN_NONE);
    assert(jsun_step(jsun, 'u') == JSUN_NONE);
    assert(jsun_step(jsun, 'l') == JSUN_NONE);
    assert(jsun_step(jsun, '_') == JSUN_ERROR);
    assert(jsun_content_length(jsun) == 0);

    jsun_init(jsun, (jsun_size_t) (jsun_content(jsun) - jsun));
    assert(jsun_step(jsun, '"') == JSUN_NONE);
    assert(jsun_step(jsun, 's') == JSUN_ERROR);

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, '"') == JSUN_NONE);
    assert(jsun_step(jsun, 's') == JSUN_NONE);
    assert(jsun_step(jsun, 't') == JSUN_NONE);
    assert(jsun_step(jsun, 'r') == JSUN_NONE);
    assert(jsun_step(jsun, 'i') == JSUN_NONE);
    assert(jsun_step(jsun, 'n') == JSUN_NONE);
    assert(jsun_step(jsun, 'g') == JSUN_NONE);
    assert(jsun_step(jsun, '"') == JSUN_STRING);
    assert(!jsun_content_equals(jsun, "\"string\""));
    assert(jsun_content_equals(jsun, "string"));

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, '"')  == JSUN_NONE);
    assert(jsun_step(jsun, '\n') == JSUN_ERROR);
    assert(jsun_content_length(jsun) == 0);

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, '"')  == JSUN_NONE);
    assert(jsun_step(jsun, 0x7f) == JSUN_ERROR);
    assert(jsun_content_length(jsun) == 0);

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, '"')  == JSUN_NONE);
    assert(jsun_step(jsun, '\\') == JSUN_NONE);
    assert(jsun_step(jsun, 'b')  == JSUN_NONE);
    assert(jsun_step(jsun, '\\') == JSUN_NONE);
    assert(jsun_step(jsun, 'f')  == JSUN_NONE);
    assert(jsun_step(jsun, '\\') == JSUN_NONE);
    assert(jsun_step(jsun, 'n')  == JSUN_NONE);
    assert(jsun_step(jsun, '\\') == JSUN_NONE);
    assert(jsun_step(jsun, 'r')  == JSUN_NONE);
    assert(jsun_step(jsun, '\\') == JSUN_NONE);
    assert(jsun_step(jsun, 't')  == JSUN_NONE);
    assert(jsun_step(jsun, '\\') == JSUN_NONE);
    assert(jsun_step(jsun, 'v')  == JSUN_NONE);
    assert(jsun_step(jsun, '\\') == JSUN_NONE);
    assert(jsun_step(jsun, '0')  == JSUN_NONE);
    assert(jsun_step(jsun, '\\') == JSUN_NONE);
    assert(jsun_step(jsun, '"')  == JSUN_NONE);
    assert(jsun_step(jsun, '"')  == JSUN_STRING);
    assert(jsun_content_equals(jsun, "\b\f\n\r\t\v\0\""));

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, '{') == JSUN_OBJECT_OPEN);
    assert(jsun_step(jsun, '"') == JSUN_NONE);
    assert(jsun_step(jsun, 'k') == JSUN_NONE);
    assert(jsun_step(jsun, 'e') == JSUN_NONE);
    assert(jsun_step(jsun, 'y') == JSUN_NONE);
    assert(jsun_step(jsun, '"') == JSUN_STRING);
    assert(jsun_step(jsun, ':') == JSUN_COLON);
    assert(jsun_step(jsun, '"') == JSUN_NONE);
    assert(jsun_step(jsun, 'v') == JSUN_NONE);
    assert(jsun_step(jsun, 'a') == JSUN_NONE);
    assert(jsun_step(jsun, 'l') == JSUN_NONE);
    assert(jsun_step(jsun, 'u') == JSUN_NONE);
    assert(jsun_step(jsun, 'e') == JSUN_NONE);
    assert(jsun_step(jsun, '"') == JSUN_STRING);
    assert(jsun_step(jsun, '}') == JSUN_OBJECT_CLOSE);

    jsun_init(jsun, sizeof(jsun));
    assert(jsun_step(jsun, '[') == JSUN_ARRAY_OPEN);
    assert(jsun_step(jsun, '{') == JSUN_OBJECT_OPEN);
    assert(jsun_step(jsun, '}') == JSUN_OBJECT_CLOSE);
    assert(jsun_step(jsun, ',') == JSUN_COMMA);
    assert(jsun_step(jsun, '{') == JSUN_OBJECT_OPEN);
    assert(jsun_step(jsun, '}') == JSUN_OBJECT_CLOSE);
    assert(jsun_step(jsun, ']') == JSUN_ARRAY_CLOSE);

    return 0;
}

