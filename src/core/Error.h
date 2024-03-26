/**
 * Created March 26, 2024
 *
 * The MIT License (MIT)
 * Copyright (c) 2024 K. Suwatchai (Mobizt)
 *
 *
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef GSHEET_CORE_ERROR_H
#define GSHEET_CORE_ERROR_H

#include <Arduino.h>

#define GSHEET_ERROR_HTTP_CODE_UNDEFINED -1000

/// HTTP codes see RFC7231
#define GSHEET_ERROR_HTTP_CODE_OK 200
#define GSHEET_ERROR_HTTP_CODE_NON_AUTHORITATIVE_INFORMATION 203
#define GSHEET_ERROR_HTTP_CODE_NO_CONTENT 204
#define GSHEET_ERROR_HTTP_CODE_MOVED_PERMANENTLY 301
#define GSHEET_ERROR_HTTP_CODE_FOUND 302
#define GSHEET_ERROR_HTTP_CODE_USE_PROXY 305
#define GSHEET_ERROR_HTTP_CODE_TEMPORARY_REDIRECT 307
#define GSHEET_ERROR_HTTP_CODE_PERMANENT_REDIRECT 308
#define GSHEET_ERROR_HTTP_CODE_BAD_REQUEST 400
#define GSHEET_ERROR_HTTP_CODE_UNAUTHORIZED 401
#define GSHEET_ERROR_HTTP_CODE_FORBIDDEN 403
#define GSHEET_ERROR_HTTP_CODE_NOT_FOUND 404
#define GSHEET_ERROR_HTTP_CODE_METHOD_NOT_ALLOWED 405
#define GSHEET_ERROR_HTTP_CODE_NOT_ACCEPTABLE 406
#define GSHEET_ERROR_HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED 407
#define GSHEET_ERROR_HTTP_CODE_REQUEST_TIMEOUT 408
#define GSHEET_ERROR_HTTP_CODE_LENGTH_REQUIRED 411
#define GSHEET_ERROR_HTTP_CODE_PRECONDITION_FAILED 412
#define GSHEET_ERROR_HTTP_CODE_PAYLOAD_TOO_LARGE 413
#define GSHEET_ERROR_HTTP_CODE_URI_TOO_LONG 414
#define GSHEET_ERROR_HTTP_CODE_MISDIRECTED_REQUEST 421
#define GSHEET_ERROR_HTTP_CODE_UNPROCESSABLE_ENTITY 422
#define GSHEET_ERROR_HTTP_CODE_TOO_MANY_REQUESTS 429
#define GSHEET_ERROR_HTTP_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE 431
#define GSHEET_ERROR_HTTP_CODE_INTERNAL_SERVER_ERROR 500
#define GSHEET_ERROR_HTTP_CODE_NOT_IMPLEMENTED 501
#define GSHEET_ERROR_HTTP_CODE_BAD_GATEWAY 502
#define GSHEET_ERROR_HTTP_CODE_SERVICE_UNAVAILABLE 503
#define GSHEET_ERROR_HTTP_CODE_GATEWAY_TIMEOUT 504
#define GSHEET_ERROR_HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED 505
#define GSHEET_ERROR_HTTP_CODE_LOOP_DETECTED 508
#define GSHEET_ERROR_HTTP_CODE_NETWORK_AUTHENTICATION_REQUIRED 511

/// TCP client errors
#define GSHEET_ERROR_TCP_CONNECTION -1
#define GSHEET_ERROR_TCP_SEND -2
#define GSHEET_ERROR_TCP_RECEIVE_TIMEOUT -3
#define GSHEET_ERROR_TCP_DISCONNECTED -4
#define GSHEET_ERROR_TCP_CLIENT_UNDEFINED -5

// GSheet error
#define GSHEET_ERROR_NETWORK_DISCONNECTED -100
#define GSHEET_ERROR_NETWORK_CONNECTION_CALLBACK -101
#define GSHEET_ERROR_NETWORK_STATUS_CALLBACK -102
#define GSHEET_ERROR_OPEN_FILE -103
#define GSHEET_ERROR_FILE_READ -104
#define GSHEET_ERROR_FILE_WRITE -105
#define GSHEET_ERROR_UNAUTHENTICATE -106
#define GSHEET_ERROR_SERVER_RESPONSE -107
#define GSHEET_ERROR_PATH_NOT_EXIST -108
#define GSHEET_ERROR_MAX_REDIRECT_REACHED -109
#define GSHEET_ERROR_TOKEN_PARSE_PK -110
#define GSHEET_ERROR_TOKEN_SIGN -111
#define GSHEET_ERROR_FW_UPDATE_TOO_LOW_FREE_SKETCH_SPACE -112
#define GSHEET_ERROR_FW_UPDATE_WRITE_FAILED -113
#define GSHEET_ERROR_FW_UPDATE_END_FAILED -114
#define GSHEET_ERROR_STREAM_TIMEOUT -115
#define GSHEET_ERROR_STREAM_AUTH_REVOKED -116
#define GSHEET_ERROR_APP_WAS_NOT_ASSIGNED -117
#define GSHEET_ERROR_OPERATION_CANCELLED -118
#define GSHEET_ERROR_TIME_IS_NOT_SET_OR_INVALID -119
#define GSHEET_ERROR_JWT_CREATION_REQUIRED -120

#if !defined(FPSTR)
#define FPSTR
#endif

class GSheetError
{
    friend class GSheetAsyncClientClass;

private:
    struct gsheet_error_info_t
    {
        String message;
        int code = 0;
    };

    gsheet_error_info_t err;

    void clearError()
    {
        err.message.remove(0, err.message.length());
        err.code = 0;
    }

    void setResponseError(const String &message, int code)
    {
        if (code == GSHEET_ERROR_HTTP_CODE_PRECONDITION_FAILED)
            err.message = FPSTR("precondition failed (ETag does not match)");
        else if (code == GSHEET_ERROR_HTTP_CODE_UNAUTHORIZED)
            err.message = FPSTR("unauthorized");
        else if (message.length())
            err.message = message;
        else
        {
            err.message = FPSTR("HTTP Status ");
            err.message += code;
        }
        err.code = code;
    }

    void setClientError(int code)
    {
        err.code = code;
        if (code < 0)
        {
            switch (code)
            {
            case GSHEET_ERROR_TCP_CONNECTION:
                err.message = FPSTR("TCP connection failed");
                break;
            case GSHEET_ERROR_TCP_SEND:
                err.message = FPSTR("TCP send failed");
                break;
            case GSHEET_ERROR_TCP_RECEIVE_TIMEOUT:
                err.message = FPSTR("TCP receive time out");
                break;
            case GSHEET_ERROR_TCP_DISCONNECTED:
                err.message = FPSTR("TCP disconnected");
                break;
            case GSHEET_ERROR_TCP_CLIENT_UNDEFINED:
                err.message = FPSTR("TCP client was undefined");
                break;
            case GSHEET_ERROR_NETWORK_DISCONNECTED:
                err.message = FPSTR("network disconnected");
                break;
            case GSHEET_ERROR_NETWORK_CONNECTION_CALLBACK:
                err.message = FPSTR("network connection callback was undefined");
                break;
            case GSHEET_ERROR_NETWORK_STATUS_CALLBACK:
                err.message = FPSTR("network status callback was undefined");
                break;
            case GSHEET_ERROR_OPEN_FILE:
                err.message = FPSTR("error opening file");
                break;
            case GSHEET_ERROR_FILE_READ:
                err.message = FPSTR("error reading file");
                break;
            case GSHEET_ERROR_FILE_WRITE:
                err.message = FPSTR("error writing file");
                break;
            case GSHEET_ERROR_UNAUTHENTICATE:
                err.message = FPSTR("unauthenticate");
                break;
            case GSHEET_ERROR_SERVER_RESPONSE:
                err.message = FPSTR("server responses ");
                break;
            case GSHEET_ERROR_PATH_NOT_EXIST:
                err.message = FPSTR("path does not exists");
                break;
            case GSHEET_ERROR_MAX_REDIRECT_REACHED:
                err.message = FPSTR("maximum redirection reaches");
                break;
            case GSHEET_ERROR_TOKEN_PARSE_PK:
                err.message = FPSTR("parse private key");
                break;
            case GSHEET_ERROR_TOKEN_SIGN:
                err.message = FPSTR("sign JWT token");
                break;
            case GSHEET_ERROR_FW_UPDATE_TOO_LOW_FREE_SKETCH_SPACE:
                err.message = FPSTR("too low sketch space");
                break;
            case GSHEET_ERROR_FW_UPDATE_WRITE_FAILED:
                err.message = FPSTR("firmware write failed");
                break;
            case GSHEET_ERROR_FW_UPDATE_END_FAILED:
                err.message = FPSTR("firmware end failed");
                break;
            case GSHEET_ERROR_STREAM_TIMEOUT:
                err.message = FPSTR("stream time out");
                break;
            case GSHEET_ERROR_STREAM_AUTH_REVOKED:
                err.message = FPSTR("auth revoked");
                break;
            case GSHEET_ERROR_APP_WAS_NOT_ASSIGNED:
                err.message = FPSTR("app was not assigned");
                break;
            case GSHEET_ERROR_OPERATION_CANCELLED:
                err.message = FPSTR("operation was cancelled");
                break;
            case GSHEET_ERROR_TIME_IS_NOT_SET_OR_INVALID:
                err.message = FPSTR("time was not set or not valid");
                break;
            default:
                err.message = FPSTR("undefined");
                break;
            }
        }
    }

public:
    GSheetError(){};
    ~GSheetError(){};
    String message() const { return err.message; }
    int code() const { return err.code; }
    void setLastError(int code, const String &msg)
    {
        err.code = code;
        err.message = msg;
    }
};

#endif