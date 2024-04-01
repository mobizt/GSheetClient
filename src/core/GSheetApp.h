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
#ifndef CORE_GSHEET_APP_H
#define CORE_GSHEET_APP_H
#include <Arduino.h>
#include "./GSheetConfig.h"
#include "./core/AuthConfig.h"
#include "./core/AsyncClient/AsyncClient.h"
#include "./core/List.h"
#if defined(GSHEET_ENABLE_JWT)
#include "./core/JWT.h"
#endif
#include "./core/Timer.h"

namespace gsheet
{

    static GSheetJWTClass GSheetJWT;

    class GSheetApp
    {
        friend class GSheetClient;

    private:
        uint16_t slot = 0;
        gsheet_async_data_item_t *sData = nullptr;
        auth_data_t auth_data;
        GSheetAsyncClientClass *aClient = nullptr;
        uint32_t aclient_addr = 0, app_addr = 0, ref_ts = 0;
        std::vector<uint32_t> aVec; // FirebaseApp vector
        std::vector<uint32_t> cVec; // AsyncClient vector
        GSheetAsyncResultCallback resultCb = NULL;
        GSheetTimer req_timer, auth_timer, err_timer;
        GSheetList vec;
        bool processing = false;
        uint32_t expire = 3600;
        GSheetJSONUtil json;
        String extras, subdomain, host;
        gsheet_slot_options_t sop;

#if defined(GSHEET_ENABLE_JWT)

        void setLastError(GSheetAsyncResult *aResult, int code, const String &message)
        {
            if (aResult)
                aResult->lastError.setLastError(code, message);
        }

#endif

        bool parseItem(GSheetStringUtil &sut, const String &src, String &dest, const String &name, const String &delim, int &p1, int &p2)
        {
            sut.parse(src, name, delim, p1, p2);
            if (p1 > -1 && p2 > -1)
            {
                if (src[p1] == '"')
                    p1++;
                if (src[p2] == '"')
                    p2--;
                dest = src.substring(p1, p2);
                p1 = p2;
                return true;
            }
            p1 = 0;
            p2 = 0;
            return false;
        }

        template <typename T = int>
        bool parseItem(GSheetStringUtil &sut, const String &src, T &dest, const String &name, const String &delim, int &p1, int &p2)
        {
            sut.parse(src, name, delim, p1, p2);
            if (p1 > -1 && p2 > -1)
            {
                if (src[p1] == '"')
                    p1++;
                if (src[p2] == '"')
                    p2--;
                dest = atoi(src.substring(p1, p2).c_str());
                p1 = p2;
                return true;
            }
            p1 = 0;
            p2 = 0;
            return false;
        }

        bool parseToken(const String &payload)
        {
            GSheetStringUtil sut;
            int p1 = 0, p2 = 0;
            auth_data.app_token.clear();
            String token, refresh;

            if (payload.indexOf("\"error\"") > -1)
            {
                String str;
                if (parseItem(sut, payload, str, "\"error\"", ",", p1, p2))
                {
                    int code = 0;
                    str = "";
                    p1 = 0;
                    p2 = 0;
                    parseItem(sut, payload, code, "\"code\"", ",", p1, p2);
                    parseItem(sut, payload, str, "\"message\"", ",", p1, p2);
                    parseItem(sut, payload, str, "\"error_description\"", "}", p1, p2);
                    if (str[str.length() - 1] == '"')
                        str[str.length() - 1] = '\0';
                    setLastError(sData ? &sData->aResult : nullptr, code, str);
                }
            }
            else if (payload.indexOf("\"idToken\"") > -1)
            {
                parseItem(sut, payload, auth_data.app_token.val[gsheet_app_tk_ns::uid], "\"localId\"", ",", p1, p2);
                p1 = 0;
                p2 = 0;
                sut.trim(auth_data.app_token.val[gsheet_app_tk_ns::uid]);
                if (parseItem(sut, payload, token, "\"idToken\"", ",", p1, p2))
                {
                    sut.trim(token);
                    parseItem(sut, payload, refresh, "\"refreshToken\"", ",", p1, p2);
                    sut.trim(refresh);
                    parseItem(sut, payload, auth_data.app_token.expire, "\"expiresIn\"", "}", p1, p2);
                }
            }
            else if (payload.indexOf("\"id_token\"") > -1)
            {
                parseItem(sut, payload, auth_data.app_token.expire, "\"expires_in\"", ",", p1, p2);
                parseItem(sut, payload, refresh, "\"refresh_token\"", ",", p1, p2);
                parseItem(sut, payload, token, "\"id_token\"", ",", p1, p2);
                parseItem(sut, payload, auth_data.app_token.val[gsheet_app_tk_ns::uid], "\"user_id\"", ",", p1, p2);
                sut.trim(refresh);
                sut.trim(token);
                sut.trim(auth_data.app_token.val[gsheet_app_tk_ns::uid]);
            }
            else if (payload.indexOf("\"access_token\"") > -1)
            {
                if (parseItem(sut, payload, token, "\"access_token\"", ",", p1, p2))
                {
                    parseItem(sut, payload, auth_data.app_token.expire, "\"expires_in\"", ",", p1, p2);
                    parseItem(sut, payload, auth_data.app_token.val[gsheet_app_tk_ns::type], "\"token_type\"", "}", p1, p2);
                }
            }

            if (token.length() > 0 && token[token.length() - 1] == '"')
                token.remove(token.length() - 1, 1);

            if (refresh.length() > 0 && refresh[refresh.length() - 1] == '"')
                refresh.remove(refresh.length() - 1, 1);

            auth_data.app_token.val[gsheet_app_tk_ns::token] = token;
            auth_data.app_token.val[gsheet_app_tk_ns::refresh] = refresh;
            auth_data.app_token.val[gsheet_app_tk_ns::pid] = auth_data.user_auth.sa.val[gsheet_sa_ns::pid];
            return token.length() > 0;
        }

        void setEvent(auth_event_type event)
        {
            auth_data.user_auth.status._event = event;

            if (event == auth_event_initializing || event == auth_event_authenticating)
                processing = true;

            if (event == auth_event_error)
            {
                err_timer.feed(5);
                auth_timer.stop();
            }

            setEventResult(sData ? &sData->aResult : nullptr, auth_data.user_auth.status.authEventString(auth_data.user_auth.status._event), auth_data.user_auth.status._event);

            if (resultCb && sData)
                resultCb(sData->aResult);

            if (event == auth_event_error || event == auth_event_ready)
            {
                processing = false;
                if (getClient())
                    stop(aClient);
                event = auth_event_uninitialized;
                clearLastError(sData ? &sData->aResult : nullptr);
                if (getClient())
                    remove(aClient);
            }
        }

        void remove(GSheetAsyncClientClass *aClient)
        {
            if (!aClient)
                return;
            aClient->handleRemove();
        }

        void clearLastError(GSheetAsyncResult *aResult)
        {
            if (aResult)
                aResult->lastError.setLastError(0, "");
        }

        GSheetAsyncClientClass *getClient()
        {
            GSheetList vec;
            return vec.existed(cVec, aclient_addr) ? aClient : nullptr;
        }

        void setEventResult(GSheetAsyncResult *aResult, const String &msg, int code)
        {
            // If aResult was not initiated, create and send temporary result to callback
            bool isRes = aResult != nullptr;

            if (!isRes)
                aResult = new GSheetAsyncResult();
            aResult->app_event.setEvent(code, msg);
            if (!isRes)
            {
                if (resultCb)
                    resultCb(*aResult);

                delete aResult;
                aResult = nullptr;
            }
        }

        void addGAPIsHost(String &str, PGM_P sub)
        {
            str += sub;
            if (str[str.length() - 1] != '.')
                str += ".";
            str += FPSTR("googleapis.com");
        }

        void addContentTypeHeader(String &header, PGM_P v)
        {
            header += FPSTR("Content-Type: ");
            header += v;
            header += FPSTR("\r\n");
        }

        void createSlot(GSheetAsyncClientClass *aClient, gsheet_slot_options_t &soption)
        {
            sData = aClient->createSlot(soption);
        }

        void newRequest(GSheetAsyncClientClass *aClient, gsheet_slot_options_t &soption, const String &subdomain, const String &extras, GSheetAsyncResultCallback resultCb, const String &uid = "")
        {
            if (!aClient)
                return;

            if (sData)
            {
                addGAPIsHost(host, subdomain.c_str());
                aClient->newRequest(sData, host, extras, "", gsheet_async_request_handler_t::http_post, soption, uid);

                addContentTypeHeader(sData->request.val[gsheet_req_hndlr_ns::header], "application/json");
                aClient->setContentLength(sData, sData->request.val[gsheet_req_hndlr_ns::payload].length());
                req_timer.feed(GSHEET_TCP_READ_TIMEOUT_SEC);
                slot = aClient->slotCount() - 1;
                sData->aResult.setDebug(FPSTR("Connecting to server..."));
                if (resultCb)
                    resultCb(sData->aResult);
            }
        }

        void process(GSheetAsyncClientClass *aClient, GSheetAsyncResult *aResult, GSheetAsyncResultCallback resultCb)
        {
            if (!aClient)
                return;

            aClient->process(true);
            aClient->handleRemove();

            if (resultCb && aResult && aResult->error().code() != 0 && aResult->error_available)
            {
                aResult->data_available = false;
                resultCb(*aResult);
            }
        }

        void stop(GSheetAsyncClientClass *aClient)
        {
            if (!aClient)
                return;
            aClient->stop(sData);
            if (sData)
            {
                delete sData;
            }
            sData = nullptr;
        }

        bool processAuth()
        {

            gsheet_sys_idle();

            if (!getClient())
                return false;

            process(aClient, sData ? &sData->aResult : nullptr, resultCb);

            if (!isExpired())
                return true;

            if (!processing)
            {
                if (auth_data.user_auth.auth_type == auth_access_token && isExpired())
                {
                    processing = true;
                    auth_data.user_auth.task_type = core_auth_task_type_refresh_token;
                    setEvent(auth_event_uninitialized);
                }
                else if ((auth_data.user_auth.status._event == auth_event_error || auth_data.user_auth.status._event == auth_event_ready) && (auth_data.app_token.expire == 0 || (auth_data.app_token.expire > 0 && isExpired())))
                {
                    processing = true;
                    setEvent(auth_event_uninitialized);
                }
            }

            if (auth_data.user_auth.jwt_signing && auth_data.user_auth.jwt_ts == 0 && err_timer.remaining() == 0)
            {
                err_timer.feed(3);
                GSheetJWT.jwt_data.err_code = GSHEET_ERROR_JWT_CREATION_REQUIRED;
                GSheetJWT.jwt_data.msg = "JWT process has not begun";
                GSheetJWT.sendErrCB(auth_data.cb, nullptr);
            }

            if (auth_data.user_auth.status._event == auth_event_uninitialized && err_timer.remaining() > 0)
                return false;

            if (auth_data.user_auth.auth_type == auth_access_token ||
                auth_data.user_auth.auth_type == auth_sa_access_token)
            {

                if (auth_data.user_auth.status._event == auth_event_uninitialized)
                {
                    if (auth_data.user_auth.auth_type == auth_access_token && auth_data.user_auth.task_type == core_auth_task_type_refresh_token)
                        setEvent(auth_event_authenticating);
                    else
                        setEvent(auth_event_initializing);
                }

                if (auth_data.user_auth.status._event == auth_event_initializing || auth_data.user_auth.status._event == auth_event_token_signing)
                {
                    gsheet_sys_idle();
#if defined(GSHEET_ENABLE_JWT)
                    if (auth_data.user_auth.sa.step == jwt_step_begin)
                    {
                        auth_data.user_auth.sa.step = jwt_step_sign;
                        if (getClient())
                            stop(aClient);

                        if (auth_data.user_auth.status._event != auth_event_token_signing)
                            setEvent(auth_event_token_signing);

                        auth_data.user_auth.jwt_signing = true;
                    }
                    else if (auth_data.user_auth.sa.step == jwt_step_sign || auth_data.user_auth.sa.step == jwt_step_ready)
                    {
                        if (GSheetJWT.ready())
                        {
                            setEvent(auth_event_authenticating);
                            auth_data.user_auth.sa.step = jwt_step_begin;
                        }
                    }
#endif
                }
                else if (auth_data.user_auth.status._event == auth_event_authenticating)
                {

                    subdomain = auth_data.user_auth.auth_type == auth_sa_access_token || auth_data.user_auth.auth_type == auth_access_token ? FPSTR("oauth2") : FPSTR("identitytoolkit");
                    sop.async = true;
                    sop.auth_used = true;

                    // Remove all slots except sse in case ServiceAuth and CustomAuth to free up memory.
                    if (getClient())
                    {
                        for (size_t i = aClient->slotCount() - 1; i == 0; i--)
                            aClient->removeSlot(i, false);

                        createSlot(aClient, sop);
                    }

                    if (auth_data.user_auth.auth_type == auth_sa_access_token)
                    {
#if defined(GSHEET_ENABLE_SERVICE_AUTH)
                        json.addObject(sData->request.val[gsheet_req_hndlr_ns::payload], "grant_type", "urn:ietf:params:oauth:grant-type:jwt-bearer", true);
                        json.addObject(sData->request.val[gsheet_req_hndlr_ns::payload], "assertion", GSheetJWT.token(), true, true);
                        GSheetJWT.clear();
#endif
                    }
                    else if (auth_data.user_auth.auth_type == auth_access_token && auth_data.user_auth.task_type == core_auth_task_type_refresh_token)
                    {
#if defined(GSHEET_ENABLE_ACCESS_TOKEN)
                        json.addObject(sData->request.val[gsheet_req_hndlr_ns::payload], "client_id", auth_data.user_auth.access_token.val[gsheet_access_tk_ns::cid], true);
                        json.addObject(sData->request.val[gsheet_req_hndlr_ns::payload], "client_secret", auth_data.user_auth.access_token.val[gsheet_access_tk_ns::csec], true);
                        json.addObject(sData->request.val[gsheet_req_hndlr_ns::payload], "grant_type", "refresh_token", true);
                        json.addObject(sData->request.val[gsheet_req_hndlr_ns::payload], "refresh_token", auth_data.user_auth.access_token.val[gsheet_access_tk_ns::refresh], true, true);
#endif
                    }

                    if (auth_data.user_auth.auth_type == auth_sa_access_token || auth_data.user_auth.auth_type == auth_access_token)
                        extras = FPSTR("/token");
                    if (getClient())
                        newRequest(aClient, sop, subdomain, extras, resultCb);
                    extras.remove(0, extras.length());
                    host.remove(0, host.length());
                    setEvent(auth_event_auth_request_sent);
                }
            }

            if (auth_data.user_auth.status._event == auth_event_auth_request_sent)
            {
                if (sData && (sData->aResult.error().code() != 0 || req_timer.remaining() == 0))
                {
                    setEvent(auth_event_error);
                    return false;
                }

                if (sData && sData->response.auth_data_available)
                {

                    setEvent(auth_event_auth_response_received);

                    if (parseToken(sData->response.val[gsheet_res_hndlr_ns::payload].c_str()))
                    {
                        sData->response.val[gsheet_res_hndlr_ns::payload].remove(0, sData->response.val[gsheet_res_hndlr_ns::payload].length());
                        auth_timer.feed(expire && expire < auth_data.app_token.expire ? expire : auth_data.app_token.expire - 2 * 60);
                        auth_data.app_token.authenticated = true;
                        if (getClient())
                            aClient->setAuthTs(millis());
                        auth_data.app_token.auth_type = auth_data.user_auth.auth_type;
                        auth_data.app_token.auth_data_type = auth_data.user_auth.auth_data_type;
                        setEvent(auth_event_ready);
                    }
                    else
                    {
                        setEvent(auth_event_error);
                    }
                }
            }

            return true;
        }

    public:
        GSheetApp()
        {
            app_addr = reinterpret_cast<uint32_t>(this);
            vec.addRemoveList(aVec, app_addr, true);
        };
        ~GSheetApp() { vec.addRemoveList(aVec, app_addr, false); };

        bool isInitialized() const { return auth_data.user_auth.initialized; }

        void loop()
        {
            auth_data.user_auth.jwt_loop = true;
            processAuth();
            auth_data.user_auth.jwt_loop = false;
        }

        bool ready() { return processAuth() && auth_data.app_token.authenticated; }

        template <typename T>
        void getApp(T &app) { app.setApp(app_addr, &auth_data.app_token, reinterpret_cast<uint32_t>(&aVec)); }

        String getToken() const { return auth_data.app_token.val[gsheet_app_tk_ns::token]; }

        String getRefreshToken() const { return auth_data.app_token.val[gsheet_app_tk_ns::refresh]; }

        String getUid() const { return auth_data.app_token.val[gsheet_app_tk_ns::uid]; }

        bool isAuthenticated() const { return auth_data.app_token.authenticated; }

        bool isExpired() { return auth_timer.remaining() == 0; }

        unsigned long ttl() { return auth_timer.remaining(); }

        void setCallback(GSheetAsyncResultCallback cb)
        {
            this->resultCb = cb;
            auth_data.cb = cb;
        }

        auth_data_t *getAuth() { return &auth_data; }
    };
};

#endif