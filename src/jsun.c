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

typedef unsigned char jsun_step_t;

enum jsun_step {
    STEP_START,
    STEP_F,
    STEP_FA,
    STEP_FAL,
    STEP_FALS,
    STEP_N,
    STEP_NU,
    STEP_NUL,
    STEP_T,
    STEP_TR,
    STEP_TRU,
    STEP_QUOTE,
    STEP_ESCAPE
};

typedef struct jsun_private jsun_private_t;

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
#define jsun_private_const(jsun) ((const jsun_private_t *) jsun)

static jsun_result_t
append(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    if (jsun->header.content_length < jsun->header.size) {
        jsun->content[jsun->header.content_length++] = value;
        result = JSUN_NONE;
    }

    return result;
}

static jsun_result_t
step_escape(jsun_private_t *jsun, jsun_char_t value) {
    jsun->header.step = STEP_QUOTE;

    switch (value) {
        case 'b': value = '\b'; break;
        case 'f': value = '\f'; break;
        case 'n': value = '\n'; break;
        case 'r': value = '\r'; break;
        case 't': value = '\t'; break;
        case 'v': value = '\v'; break;
        case '0': value = '\0'; break;
        default: break;
    }

    return append(jsun, value);
}

static jsun_result_t
step_quote(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;

    if (value == '"') {
        jsun->header.step = STEP_START;
        result = JSUN_STRING;
    }
    else if (value == '\\') {
        jsun->header.step = STEP_ESCAPE;
        result = JSUN_NONE;
    }
    else if ((' ' <= value) && (value <= '~')) {
        result = append(jsun, value);
    }

    return result;
}

static jsun_result_t
step_tru(jsun_private_t *jsun, jsun_char_t value) {
    jsun->header.step = STEP_START;
    return (value == 'e') ? JSUN_TRUE : JSUN_ERROR;
}

static jsun_result_t
step_tr(jsun_private_t *jsun, jsun_char_t value) {
    jsun->header.step = STEP_TRU;
    return (value == 'u') ? JSUN_NONE : JSUN_ERROR;
}

static jsun_result_t
step_t(jsun_private_t *jsun, jsun_char_t value) {
    jsun->header.step = STEP_TR;
    return (value == 'r') ? JSUN_NONE : JSUN_ERROR;
}

static jsun_result_t
step_nul(jsun_private_t *jsun, jsun_char_t value) {
    jsun->header.step = STEP_START;
    return (value == 'l') ? JSUN_NULL : JSUN_ERROR;
}

static jsun_result_t
step_nu(jsun_private_t *jsun, jsun_char_t value) {
    jsun->header.step = STEP_NUL;
    return (value == 'l') ? JSUN_NONE : JSUN_ERROR;
}

static jsun_result_t
step_n(jsun_private_t *jsun, jsun_char_t value) {
    jsun->header.step = STEP_NU;
    return (value == 'u') ? JSUN_NONE : JSUN_ERROR;
}

static jsun_result_t
step_fals(jsun_private_t *jsun, jsun_char_t value) {
    jsun->header.step = STEP_START;
    return (value == 'e') ? JSUN_FALSE : JSUN_ERROR;
}

static jsun_result_t
step_fal(jsun_private_t *jsun, jsun_char_t value) {
    jsun->header.step = STEP_FALS;
    return (value == 's') ? JSUN_NONE : JSUN_ERROR;
}

static jsun_result_t
step_fa(jsun_private_t *jsun, jsun_char_t value) {
    jsun->header.step = STEP_FAL;
    return (value == 'l') ? JSUN_NONE : JSUN_ERROR;
}

static jsun_result_t
step_f(jsun_private_t *jsun, jsun_char_t value) {
    jsun->header.step = STEP_FA;
    return (value == 'a') ? JSUN_NONE : JSUN_ERROR;
}

static jsun_result_t
step_start(jsun_private_t *jsun, jsun_char_t value) {
    jsun_result_t result = JSUN_ERROR;
    jsun->header.content_length = 0;

    switch (value) {
        case 'f':
            jsun->header.step = STEP_F;
            result = JSUN_NONE;
            break;

        case 'n':
            jsun->header.step = STEP_N;
            result = JSUN_NONE;
            break;

        case 't':
            jsun->header.step = STEP_T;
            result = JSUN_NONE;
            break;

        case '"':
            jsun->header.step = STEP_QUOTE;
            result = JSUN_NONE;
            break;

        case '[':
            result = JSUN_ARRAY_OPEN;
            break;

        case ']':
            result = JSUN_ARRAY_CLOSE;
            break;

        case '{':
            result = JSUN_OBJECT_OPEN;
            break;

        case '}':
            result = JSUN_OBJECT_CLOSE;
            break;

        case ':':
            result = JSUN_COLON;
            break;

        case ',':
            result = JSUN_COMMA;
            break;

        case '\f':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
        case  ' ':
            result = JSUN_NONE;
            break;

        default:
            break;
    }

    return result;
}

typedef jsun_result_t (*jsun_fun_t)(jsun_private_t *jsun, jsun_char_t value);

static const jsun_fun_t step_functions[] = {
    step_start,
    step_f,
    step_fa,
    step_fal,
    step_fals,
    step_n,
    step_nu,
    step_nul,
    step_t,
    step_tr,
    step_tru,
    step_quote,
    step_escape
};

void
jsun_init(jsun_t *jsun, jsun_size_t size) {
    jsun_private_t *private = jsun_private(jsun);
    private->header.step = STEP_START;
    private->header.size = (jsun_size_t) (size - sizeof(jsun_header_t));
    private->header.content_length = 0;
}

jsun_result_t
jsun_step(jsun_t *jsun, jsun_char_t value) {
    jsun_private_t *private = jsun_private(jsun);
    return step_functions[private->header.step](private, value);
}

const jsun_char_t *
jsun_content(const jsun_t *jsun) {
    return jsun_private_const(jsun)->content;
}

jsun_size_t
jsun_content_length(const jsun_t *jsun) {
    return jsun_private_const(jsun)->header.content_length;
}

jsun_size_t
jsun_content_equals(const jsun_t *jsun, const jsun_char_t *content) {
    jsun_size_t i;
    jsun_size_t length = jsun_content_length(jsun);

    for (i = 0; i < length; i++) {
        if (jsun_content(jsun)[i] != content[i]) {
            return 0;
        }
    }

    return (content[i] == '\0');
}
