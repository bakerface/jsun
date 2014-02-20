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

typedef struct jsun_private jsun_private_t;
typedef jsun_result_t (*jsun_step_t)(jsun_private_t *jsun, jsun_char_t value);

typedef struct jsun_header {
    jsun_step_t step;
    jsun_size_t size;
    jsun_size_t content_length;
} jsun_header_t;

struct jsun_private {
    jsun_header_t header;
    jsun_char_t content[1];
};

#define jsun_private(jsun) ((jsun_private_t *) jsun)

static jsun_result_t
step(jsun_private_t *jsun, jsun_char_t value);

static jsun_result_t
step_tru(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    switch (value) {
        case 'e':
            jsun->header.step = step;
            result = JSUN_TRUE;
            break;

        default:
            break;
    }

    return result;
}

static jsun_result_t
step_tr(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    switch (value) {
        case 'u':
            jsun->header.step = step_tru;
            result = JSUN_NONE;
            break;

        default:
            break;
    }

    return result;
}

static jsun_result_t
step_t(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    switch (value) {
        case 'r':
            jsun->header.step = step_tr;
            result = JSUN_NONE;
            break;

        default:
            break;
    }

    return result;
}

static jsun_result_t
step_fals(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    switch (value) {
        case 'e':
            jsun->header.step = step;
            result = JSUN_FALSE;
            break;

        default:
            break;
    }

    return result;
}

static jsun_result_t
step_fal(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    switch (value) {
        case 's':
            jsun->header.step = step_fals;
            result = JSUN_NONE;
            break;

        default:
            break;
    }

    return result;
}

static jsun_result_t
step_fa(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    switch (value) {
        case 'l':
            jsun->header.step = step_fal;
            result = JSUN_NONE;
            break;

        default:
            break;
    }

    return result;
}

static jsun_result_t
step_f(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    switch (value) {
        case 'a':
            jsun->header.step = step_fa;
            result = JSUN_NONE;
            break;

        default:
            break;
    }

    return result;
}

static jsun_result_t
step_nul(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    switch (value) {
        case 'l':
            jsun->header.step = step;
            result = JSUN_NULL;
            break;

        default:
            break;
    }

    return result;
}

static jsun_result_t
step_nu(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    switch (value) {
        case 'l':
            jsun->header.step = step_nul;
            result = JSUN_NONE;
            break;

        default:
            break;
    }

    return result;
}

static jsun_result_t
step_n(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    switch (value) {
        case 'u':
            jsun->header.step = step_nu;
            result = JSUN_NONE;
            break;

        default:
            break;
    }

    return result;
}

jsun_result_t
step(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    switch (value) {
        case 'f':
            jsun->header.step = step_f;
            result = JSUN_NONE;
            break;

        case 'n':
            jsun->header.step = step_n;
            result = JSUN_NONE;
            break;

        case 't':
            jsun->header.step = step_t;
            result = JSUN_NONE;
            break;

        default:
            break;
    }

    return result;
}

void
jsun_init(jsun_t *jsun, jsun_size_t size) {
    jsun_private_t *private = jsun_private(jsun);
    private->header.step = step;
    private->header.size = size - sizeof(jsun_header_t);
    private->header.content_length = 0;
}

jsun_result_t
jsun_step(jsun_t *jsun, jsun_char_t value) {
    jsun_private_t *private = jsun_private(jsun);
    return private->header.step(private, value);
}

jsun_char_t *
jsun_content(jsun_t *jsun) {
    return jsun_private(jsun)->content;
}

jsun_size_t
jsun_content_length(jsun_t *jsun) {
    return jsun_private(jsun)->header.content_length;
}

