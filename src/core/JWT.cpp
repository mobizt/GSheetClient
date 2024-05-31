/**
 * Created May 30, 2024
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
#ifndef JWT_CPP
#define JWT_CPP

#include <Arduino.h>
#include "./GSheetConfig.h"
#include "./core/Memory.h"
#include "./core/AuthConfig.h"
#include "./core/Base64.h"
#include "./core/URL.h"
#include "./core/JSON.h"
#include "./core/Error.h"
#include "./core/Core.h"

#if defined(GSHEET_ENABLE_JWT)

#include "JWT.h"

#if defined(USE_EMBED_SSL_ENGINE) && !defined(CORE_MOCK)
extern "C"
{
#if defined(ESP8266)
    make_stack_thunk(br_rsa_i15_pkcs1_sign);
#endif
};
#endif

GSheetJWTClass::GSheetJWTClass()
{
    err_timer.feed(1);
#if defined(USE_EMBED_SSL_ENGINE)
    stack_thunk_add_ref();
#endif
}

GSheetJWTClass::~GSheetJWTClass()
{
#if defined(USE_EMBED_SSL_ENGINE)
    stack_thunk_del_ref();
#endif
}

const char *GSheetJWTClass::token() { return jwt_data.token.c_str(); }

void GSheetJWTClass::clear()
{
    jwt_data.token.remove(0, jwt_data.token.length());
    jwt_data.pk.remove(0, jwt_data.pk.length());
    payload.remove(0, payload.length());
    if (this->auth_data)
    {
        this->auth_data->user_auth.sa.step = gsheet_jwt_step_begin;
        this->auth_data->user_auth.jwt_ts = 0;
        this->auth_data->user_auth.jwt_time_debug = false;
    }
    processing = false;
}

bool GSheetJWTClass::ready()
{
    return this->auth_data && this->auth_data->user_auth.sa.step == gsheet_jwt_step_ready;
}
bool GSheetJWTClass::loop(auth_data_t *auth_data)
{
    if (auth_data && auth_data->user_auth.jwt_signing)
    {
        bool ret = begin(auth_data);
        if (ret)
            ret = create();
        if (!ret)
        {
            if (auth_data->refResult)
                sendErrResult(auth_data->refResult);
            else
                sendErrCB(auth_data ? auth_data->cb : NULL, nullptr);
        }
        return ret;
    }
    return false;
}

void GSheetJWTClass::sendErrCB(GSheetAsyncResultCallback cb, GSheetAsyncResult *aResult)
{
    if (cb)
    {
        if (err_timer.remaining() == 0)
        {
            err_timer.feed(5);
            bool hasRes = aResult != nullptr;
            if (!hasRes)
                aResult = new GSheetAsyncResult();
            aResult->error().setLastError(jwt_data.err_code, jwt_data.msg);
            cb(*aResult);
            if (!hasRes)
            {
                delete aResult;
                aResult = nullptr;
            }
        }
    }
}

void GSheetJWTClass::sendErrResult(GSheetAsyncResult *refResult)
{
    if (refResult)
        refResult->error().setLastError(jwt_data.err_code, jwt_data.msg);
}

void GSheetJWTClass::setAppDebug(gsheet_app_debug_t *app_debug)
{
    this->app_debug = app_debug;
}

bool GSheetJWTClass::begin(auth_data_t *auth_data)
{
    if (processing || !auth_data)
        return false;

    if (auth_data->user_auth.sa.step == gsheet_jwt_step_begin)
    {
        processing = true;
        this->auth_data = auth_data;
        this->auth_data->app_token.clear();
        this->auth_data->user_auth.jwt_ts = millis();
        this->auth_data->user_auth.sa.step = gsheet_jwt_step_create_jwt;
        processing = false;
    }

    return true;
}

bool GSheetJWTClass::create()
{

#if !defined(USE_LEGACY_TOKEN_ONLY) && !defined(GSHEET_USE_LEGACY_TOKEN_ONLY)

    if (!auth_data)
        return exit(false);

    if (auth_data->user_auth.sa.step == gsheet_jwt_step_create_jwt)
    {

        uint32_t now = 0;
        if (auth_data->user_auth.timestatus_cb)
        {

            if (app_debug && !auth_data->user_auth.jwt_time_debug)
            {
                auth_data->user_auth.jwt_time_debug = true;
                app_debug->setDebug(FPSTR("Getting current time..."));
                return exit(false);
            }
            auth_data->user_auth.timestatus_cb(now);
        }

        if (now < GSHEET_DEFAULT_TS)
        {
            jwt_data.err_code = GSHEET_ERROR_TIME_IS_NOT_SET_OR_INVALID;
            jwt_data.msg = (const char *)FPSTR("JWT, time was not set or not valid");
            return exit(false);
        }

        // header
        // {"alg":"RS256","typ":"JWT"}

        size_t len = 0;
        jwt_data.token = FPSTR("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9");

        // payload
        // {"iss":"<email>","sub":"<email>","aud":"<audience>","iat":<timstamp>,"exp":<expire>,"scope":"<scope>"}
        // {"iss":"<email>","sub":"<email>","aud":"<audience>","iat":<timstamp>,"exp":<expire>,"uid":"<uid>","claims":"<claims>"}

        json.addObject(payload, "iss", auth_data->user_auth.sa.val[gsheet_sa_ns::cm], true);
        json.addObject(payload, "sub", auth_data->user_auth.sa.val[gsheet_sa_ns::cm], true);

        String t = FPSTR("https://");
        if (auth_data->user_auth.auth_type == gsheet_auth_sa_access_token)
        {
            jwt_add_gapis_host(t, "oauth2");
            t += FPSTR("/token");
        }

        json.addObject(payload, "aud", t, true);
        t.remove(0, t.length());
        json.addObject(payload, "iat", String(now), false);
        json.addObject(payload, "exp", String((int)(now + 3600)), false);

        if (auth_data->user_auth.auth_type == gsheet_auth_sa_access_token)
        {
            String buri;
            String host;
            jwt_add_gapis_host(host, "www");
            uut.host2Url(buri, host);
            buri += FPSTR("/auth/");

            String s = buri; // https://www.googleapis.com/auth/
            s += FPSTR("devstorage.full_control");
            jwt_add_sp(s);
            s += buri; // https://www.googleapis.com/auth/
            s += FPSTR("datastore");
            jwt_add_sp(s);
            s += buri; // https://www.googleapis.com/auth/
            s += FPSTR("userinfo.email");
            jwt_add_sp(s);
            s += buri; // https://www.googleapis.com/auth/
            s += FPSTR("firebase.database");
            jwt_add_sp(s);
            s += buri; // https://www.googleapis.com/auth/
            s += FPSTR("cloud-platform");
            jwt_add_sp(s);
            s += buri; // https://www.googleapis.com/auth/
            s += FPSTR("iam");
            buri.remove(0, buri.length());

            json.addObject(payload, "scope", s, true, true);
        }

        len = but.encodedLength(payload.length());
        char *buf = reinterpret_cast<char *>(mem.alloc(len));
        but.encodeUrl(mem, buf, (unsigned char *)payload.c_str(), payload.length());
        payload.remove(0, payload.length());
        jwt_data.token += '.';
        jwt_data.token += buf;
        mem.release(&buf);

        // create message digest from encoded header and payload
        br_sha256_context mc;
        br_sha256_init(&mc);
        br_sha256_update(&mc, jwt_data.token.c_str(), jwt_data.token.length());
        if (!jwt_data.hash)
            jwt_data.hash = reinterpret_cast<char *>(mem.alloc(32));
        br_sha256_out(&mc, jwt_data.hash);
        jwt_data.token += '.';

        auth_data->user_auth.sa.step = gsheet_jwt_step_sign;
    }
    else if (auth_data->user_auth.sa.step == gsheet_jwt_step_sign)
    {
        // RSA private key
        PrivateKey *pk = nullptr;
        gsheet_sys_idle();
        
        // parse priv key
        if (jwt_data.pk.length() > 0)
            pk = new PrivateKey(jwt_data.pk.c_str());
        else if (auth_data->user_auth.sa.val[gsheet_sa_ns::pk].length() > 0)
            pk = new PrivateKey(auth_data->user_auth.sa.val[gsheet_sa_ns::pk].c_str());

        jwt_data.pk.remove(0, jwt_data.pk.length());

        if (!pk)
        {
            jwt_data.err_code = GSHEET_ERROR_TOKEN_PARSE_PK;
            jwt_data.msg = (const char *)FPSTR("JWT, private key parsing fail");
            auth_data->user_auth.sa.step = gsheet_jwt_step_error;
            return exit(false);
        }

        if (!pk->isRSA())
        {
            delete pk;
            pk = nullptr;
            jwt_data.err_code = GSHEET_ERROR_TOKEN_PARSE_PK;
            jwt_data.msg = (const char *)FPSTR("JWT, invalid RSA private key");
            auth_data->user_auth.sa.step = gsheet_jwt_step_error;
            return exit(false);
        }

        const br_rsa_private_key *br_rsa_key = pk->getRSA();

        // generate RSA signature from private key and message digest
        gsheet_sys_idle();
        if (!jwt_data.signature)
            jwt_data.signature = reinterpret_cast<unsigned char *>(mem.alloc(256));
        int ret = br_rsa_i15_pkcs1_sign(BR_HASH_OID_SHA256, (const unsigned char *)jwt_data.hash, br_sha256_SIZE, br_rsa_key, jwt_data.signature);
        gsheet_sys_idle();

        if (jwt_data.hash)
            mem.release(&jwt_data.hash);

        size_t len = but.encodedLength(256);
        char *buf = reinterpret_cast<char *>(mem.alloc(len));
        but.encodeUrl(mem, buf, jwt_data.signature, 256);

        delete pk;
        pk = nullptr;

        // get the signed JWT
        if (ret > 0)
        {
            jwt_data.token += buf;
            mem.release(&buf);
            auth_data->user_auth.sa.step = gsheet_jwt_step_ready;
        }
        else
        {
            mem.release(&buf);
            jwt_data.err_code = GSHEET_ERROR_TOKEN_SIGN;
            jwt_data.msg = (const char *)FPSTR("JWT, token signing fail");
            auth_data->user_auth.sa.step = gsheet_jwt_step_error;
            return exit(false);
        }
    }

#endif

    return exit(true);
}

#endif

#endif