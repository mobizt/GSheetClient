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
#include "./core/AppBase.h"

namespace gsheet
{

#if defined(GSHEET_ENABLE_JWT)
    static GSheetJWTClass GSheetJWT;
#endif

    class GSheetApp : public GSheetAppBase, public GSheetResultBase
    {
        friend class GSheetClient;

    private:
        uint16_t slot = 0;
        gsheet_async_data_item_t *sData = nullptr;
        auth_data_t auth_data;
        GSheetAsyncClientClass *aClient = nullptr;
        uint32_t aclient_addr = 0, app_addr = 0, ref_ts = 0;
        std::vector<uint32_t> aVec; // GSheetApp vector
        std::vector<uint32_t> cVec; // GSheetAsyncClient vector
        GSheetAsyncResultCallback resultCb = NULL;
        GSheetAsyncResult *refResult = nullptr;
        uint32_t ref_result_addr = 0;
        GSheetTimer req_timer, auth_timer, err_timer, app_ready_timer;
        bool deinit = false;
        GSheetList vec;
        bool processing = false;
        uint32_t expire = GSHEET_DEFAULT_TOKEN_TTL;
        GSheetJSONUtil json;
        String extras, subdomain, host;
        gsheet_slot_options_t sop;
        String uid;
#if defined(GSHEET_ENABLE_JWT)
        GSheetJWTClass *jwtClass = nullptr;
#endif

#if defined(GSHEET_ENABLE_JWT)

        void setLastError(GSheetAsyncResult *aResult, int code, const String &message)
        {
            setLastErrorBase(aResult, code, message);
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

        GSheetAsyncClientClass *getClient()
        {
            GSheetList vec;
            return vec.existed(cVec, aclient_addr) ? aClient : nullptr;
        }

        void setEvent(gsheet_auth_event_type event)
        {
            if (auth_data.user_auth.status._event == event)
                return;

            auth_data.user_auth.status._event = event;

            if (event == gsheet_auth_event_initializing || event == gsheet_auth_event_authenticating)
                processing = true;

            if (event == gsheet_auth_event_error)
            {
                err_timer.feed(5);
                auth_timer.stop();
            }

            setEventResult(sData ? &sData->aResult : getRefResult(), auth_data.user_auth.status.authEventString(auth_data.user_auth.status._event), auth_data.user_auth.status._event);

            if (event == gsheet_auth_event_error || event == gsheet_auth_event_ready)
            {
                processing = false;
                event = gsheet_auth_event_uninitialized;
                clearLastError(sData ? &sData->aResult : nullptr);
                if (getClient())
                    stop(aClient);
            }
        }

        void clearLastError(GSheetAsyncResult *aResult)
        {
            clearLastErrorBase(aResult);
        }

        void setEventResult(GSheetAsyncResult *aResult, const String &msg, int code)
        {
            // If aResult was not initiated, create and send temporary result to callback
            bool isRes = aResult != nullptr;

            // Set uid from user defined async result.
            if (getRefResult())
                uid = refResult->uid();

            if (!isRes)
            {
                aResult = new GSheetAsyncResult();
                resultSetEvent(aResult, getAppEvent(aClient));
                resultSetDebug(aResult, getAppDebug(aClient));

                // Store the uid;
                if (uid.length() == 0)
                    uid = aResult->uid();
                else
                    setResultUID(aResult, uid);
            }

            if (!getRefResult())
                resetEvent(*getAppEvent(aClient));

            setEventBase(*getAppEvent(aClient), code, msg);

            if (resultCb)
                resultCb(*aResult);

            if (!isRes)
            {
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
            if (aClient)
                sData = createSlotBase(aClient, soption);
        }

        GSheetAsyncResult *getRefResult()
        {
            GSheetList vec;
            return aClient && vec.existed(getRVec(aClient), ref_result_addr) ? refResult : nullptr;
        }

        void setRefResult(GSheetAsyncResult *refResult, uint32_t rvec_addr)
        {
            this->refResult = refResult;
            ref_result_addr = reinterpret_cast<uint32_t>(refResult);
            setRVec(this->refResult, rvec_addr);
            if (rvec_addr > 0)
            {
                std::vector<uint32_t> *rVec = reinterpret_cast<std::vector<uint32_t> *>(rvec_addr);
                GSheetList vec;
                vec.addRemoveList(*rVec, ref_result_addr, true);
            }
        }

        void newRequest(GSheetAsyncClientClass *aClient, gsheet_slot_options_t &soption, const String &subdomain, const String &extras, GSheetAsyncResultCallback resultCb, const String &uid = "")
        {
            if (!aClient)
                return;

            gsheet_sys_idle();

            if (sData)
            {
                addGAPIsHost(host, subdomain.c_str());
                newRequestBase(aClient, sData, host, extras, "", gsheet_async_request_handler_t::http_post, soption, uid);

                addContentTypeHeader(sData->request.val[gsheet_req_hndlr_ns::header], "application/json");
                setContentLengthBase(aClient, sData, sData->request.val[gsheet_req_hndlr_ns::payload].length());
                req_timer.feed(GSHEET_TCP_READ_TIMEOUT_SEC);
                slot = slotCountBase(aClient) - 1;

                setDebugBase(*getAppDebug(aClient), FPSTR("Connecting to server..."));

                if (resultCb)
                    resultCb(sData->aResult);
            }
        }

        void process(GSheetAsyncClientClass *aClient, GSheetAsyncResult *aResult, GSheetAsyncResultCallback resultCb)
        {
            if (!aClient)
                return;

            processBase(aClient, true);
            handleRemoveBase(aClient);
        }

        // Stop client and remove slot
        void stop(GSheetAsyncClientClass *aClient)
        {
            if (!aClient)
                return;

            stopAsync(aClient, sData);

            if (sData)
            {
                removeSlotBase(aClient, slot, false);
                if (sData)
                    delete sData;
                sData = nullptr;
            }

            sData = nullptr;
        }

        bool processAuth()
        {

            gsheet_sys_idle();

            if (!getClient())
                return false;

            // Deinitialize
            if (deinit && auth_data.user_auth.status._event == gsheet_auth_event_uninitialized)
            {
                if (auth_data.user_auth.initialized)
                {
                    stop(aClient);
                    deinitializeApp();
                    auth_timer.stop();
                }
                return false;
            }

            updateDebug(*getAppDebug(aClient));
            updateEvent(*getAppEvent(aClient));

            process(aClient, sData ? &sData->aResult : nullptr, resultCb);

            if (!isExpired())
                return true;

            if (!processing)
            {
                if (auth_data.user_auth.auth_type == gsheet_auth_access_token && isExpired())
                {
                    processing = true;
                    auth_data.user_auth.task_type = gsheet_core_auth_task_type_refresh_token;
                    setEvent(gsheet_auth_event_uninitialized);
                }
                else if ((auth_data.user_auth.status._event == gsheet_auth_event_error || auth_data.user_auth.status._event == gsheet_auth_event_ready) && (auth_data.app_token.expire == 0 || (auth_data.app_token.expire > 0 && isExpired())))
                {
                    processing = true;
                    setEvent(gsheet_auth_event_uninitialized);
                }
            }

            if (auth_data.user_auth.jwt_signing && auth_data.user_auth.jwt_ts == 0)
            {
#if defined(GSHEET_ENABLE_JWT)
                if (err_timer.remaining() == 0)
                {
                    err_timer.feed(3);
                    jwtProcessor()->jwt_data.err_code = GSHEET_ERROR_JWT_CREATION_REQUIRED;
                    jwtProcessor()->jwt_data.msg = "JWT process has not begun";
                    if (getRefResult())
                        jwtProcessor()->sendErrResult(auth_data.refResult);
                    else
                        jwtProcessor()->sendErrCB(auth_data.cb, nullptr);
                }
#endif
                return false;
            }

            if (auth_data.user_auth.status._event == gsheet_auth_event_uninitialized && err_timer.remaining() > 0)
                return false;

            if (auth_data.user_auth.auth_type == gsheet_auth_access_token ||
                auth_data.user_auth.auth_type == gsheet_auth_sa_access_token)
            {

                if (auth_data.user_auth.status._event == gsheet_auth_event_uninitialized)
                {
                    if (auth_data.user_auth.auth_type == gsheet_auth_access_token && auth_data.user_auth.task_type == gsheet_core_auth_task_type_refresh_token)
                        setEvent(gsheet_auth_event_authenticating);
                    else
                        setEvent(gsheet_auth_event_initializing);
                }

                if (auth_data.user_auth.status._event == gsheet_auth_event_initializing || auth_data.user_auth.status._event == gsheet_auth_event_token_signing)
                {
                    gsheet_sys_idle();
#if defined(GSHEET_ENABLE_JWT)
                    if (auth_data.user_auth.sa.step == gsheet_jwt_step_begin)
                    {
                        if (getClient())
                            stop(aClient);

                        if (auth_data.user_auth.status._event != gsheet_auth_event_token_signing)
                            setEvent(gsheet_auth_event_token_signing);

                        auth_data.user_auth.jwt_signing = true;

                        jwtProcessor()->begin(&auth_data);
                    }
                    else if (auth_data.user_auth.sa.step == gsheet_jwt_step_sign || auth_data.user_auth.sa.step == gsheet_jwt_step_ready)
                    {
                        if (jwtProcessor()->ready())
                        {
                            auth_data.user_auth.jwt_signing = false;
                            setEvent(gsheet_auth_event_authenticating);
                            auth_data.user_auth.sa.step = gsheet_jwt_step_begin;
                        }
                    }
#endif
                }
                else if (auth_data.user_auth.status._event == gsheet_auth_event_authenticating)
                {

                    subdomain = auth_data.user_auth.auth_type == gsheet_auth_sa_access_token || auth_data.user_auth.auth_type == gsheet_auth_access_token ? FPSTR("oauth2") : FPSTR("identitytoolkit");
                    sop.async = true;
                    sop.auth_used = true;

                    // Remove all slots except sse in case ServiceAuth and CustomAuth to free up memory.
                    if (getClient())
                    {
                        for (size_t i = slotCountBase(aClient) - 1; i == 0; i--)
                            removeSlotBase(aClient, i, false);

                        createSlot(aClient, sop);
                    }

                    if (auth_data.user_auth.auth_type == gsheet_auth_sa_access_token)
                    {
#if defined(GSHEET_ENABLE_SERVICE_AUTH)
                        json.addObject(sData->request.val[gsheet_req_hndlr_ns::payload], "grant_type", "urn:ietf:params:oauth:grant-type:jwt-bearer", true);
#if defined(GSHEET_ENABLE_JWT)
                        json.addObject(sData->request.val[gsheet_req_hndlr_ns::payload], "assertion", jwtProcessor()->token(), true, true);
                        jwtProcessor()->clear();
#endif
#endif
                    }
                    else if (auth_data.user_auth.auth_type == gsheet_auth_access_token && auth_data.user_auth.task_type == gsheet_core_auth_task_type_refresh_token)
                    {
#if defined(GSHEET_ENABLE_ACCESS_TOKEN)
                        json.addObject(sData->request.val[gsheet_req_hndlr_ns::payload], "client_id", auth_data.user_auth.access_token.val[gsheet_access_tk_ns::cid], true);
                        json.addObject(sData->request.val[gsheet_req_hndlr_ns::payload], "client_secret", auth_data.user_auth.access_token.val[gsheet_access_tk_ns::csec], true);
                        json.addObject(sData->request.val[gsheet_req_hndlr_ns::payload], "grant_type", "refresh_token", true);
                        json.addObject(sData->request.val[gsheet_req_hndlr_ns::payload], "refresh_token", auth_data.user_auth.access_token.val[gsheet_access_tk_ns::refresh], true, true);
#endif
                    }

                    if (auth_data.user_auth.auth_type == gsheet_auth_sa_access_token || auth_data.user_auth.auth_type == gsheet_auth_access_token)
                        extras = FPSTR("/token");

                    if (getClient())
                        newRequest(aClient, sop, subdomain, extras, resultCb);

                    extras.remove(0, extras.length());
                    host.remove(0, host.length());
                    setEvent(gsheet_auth_event_auth_request_sent);
                }
            }

            if (auth_data.user_auth.status._event == gsheet_auth_event_auth_request_sent || auth_data.user_auth.status._event == gsheet_auth_event_auth_response_received)
            {
                gsheet_sys_idle();

                if (sData && ((sData->response.payloadLen > 0 && sData->aResult.error().code() != 0) || req_timer.remaining() == 0))
                {
                    // In case of googleapis returns http status code >= 400 or request is timed out.
                    // Note that, only status line was read in case http status code >= 400
                    setEvent(gsheet_auth_event_error);
                    return false;
                }

                if (sData && sData->response.auth_data_available)
                {

                    if (auth_data.user_auth.status._event != gsheet_auth_event_auth_response_received)
                    {
                        setEvent(gsheet_auth_event_auth_response_received);
                        if (getRefResult())
                            return false;
                    }

                    if (parseToken(sData->response.val[gsheet_res_hndlr_ns::payload].c_str()))
                    {
                        sData->response.val[gsheet_res_hndlr_ns::payload].remove(0, sData->response.val[gsheet_res_hndlr_ns::payload].length());
                        auth_timer.feed(expire && expire < auth_data.app_token.expire ? expire : auth_data.app_token.expire - 2 * 60);
                        auth_data.app_token.authenticated = true;
                        if (getClient())
                            setAuthTsBase(aClient, millis());
                        auth_data.app_token.auth_type = auth_data.user_auth.auth_type;
                        auth_data.app_token.auth_data_type = auth_data.user_auth.auth_data_type;
                        setEvent(gsheet_auth_event_ready);
                        app_ready_timer.feed(1);
                    }
                    else
                    {
                        setEvent(gsheet_auth_event_error);
                    }
                }
            }

            // Defer the ready status to allow the remaining information to be read or printed inside the waiting loop.
            if (auth_data.user_auth.status._event == gsheet_auth_event_ready && auth_data.app_token.authenticated && app_ready_timer.remaining() > 0)
                return false;

            return true;
        }

        void deinitializeApp()
        {
            auth_data.app_token.clear();
            auth_data.user_auth.clear();
        }

#if defined(GSHEET_ENABLE_JWT)
        GSheetJWTClass *jwtProcessor() { return jwtClass ? jwtClass : &GSheetJWT; }
#endif

    public:
        GSheetApp()
        {
            app_addr = reinterpret_cast<uint32_t>(this);
            vec.addRemoveList(aVec, app_addr, true);
        };
        ~GSheetApp()
        {
            if (sData)
                delete sData;
            sData = nullptr;
            vec.addRemoveList(aVec, app_addr, false);
        };

        /**
         * Get the app initialization status.
         *
         * @return bool Return true if initialized.
         */
        bool isInitialized() const { return auth_data.user_auth.initialized; }

        /**
         * The authentication/authorization handler.
         */
        void loop()
        {
            auth_data.user_auth.jwt_loop = true;
            processAuth();
            auth_data.user_auth.jwt_loop = false;
        }

        /**
         * Get the authentication/autorization process status.
         *
         * @return bool Return true if the auth process was finished. Returns false if isExpired() returns true.
         */
        bool ready() { return processAuth() && auth_data.app_token.authenticated; }

        /**
         * Appy the authentication/authorization credentials to the Firebase services app.
         *
         * @param app The Firebase services calss object e.g. RealtimeDatabase, Storage, Messaging, CloudStorage and CloudFunctions.
         */
        template <typename T>
        void getApp(T &app) { setAppBase(app, app_addr, &auth_data.app_token, reinterpret_cast<uint32_t>(&aVec)); }

        /**
         * Get the auth token.
         *
         * @return String of auth tokens based on the authentication/authoeization e.g. ID token and access token.
         */
        String getToken() const { return auth_data.app_token.val[gsheet_app_tk_ns::token]; }

        /**
         * Get the refresh token.
         *
         * @return String of refresh token after user sign in or authorization using ID token.
         */
        String getRefreshToken() const { return auth_data.app_token.val[gsheet_app_tk_ns::refresh]; }

        /**
         * Get unique identifier.
         *
         * @return String of unique identifier after user sign in or authorization using ID token.
         */
        String getUid() const { return auth_data.app_token.val[gsheet_app_tk_ns::uid]; }

        /**
         * Get the authentication status since app initialized.
         *
         * @return bool Return true once authenticated since initialized. It will reset when app re-initialized and user management task was executed.
         */
        bool isAuthenticated() const { return auth_data.app_token.authenticated; }

        /**
         * Get the auth token expiration status.
         *
         * @return bool Return true if auth token was expired upon the expire period setup.
         */
        bool isExpired() { return auth_timer.remaining() == 0; }

        /**
         * Get the remaining time to live of token until expired.
         *
         * @return integer value of ttl.
         */
        unsigned long ttl() { return auth_timer.remaining(); }

        /**
         * Set the async result callback function.
         *
         * @param cb The async result callback function (AsyncResultCallback).
         */
        void setCallback(GSheetAsyncResultCallback cb)
        {
            this->resultCb = cb;
            auth_data.cb = cb;
        }

        /**
         * Set the async result class object.
         *
         * @param aResult The async result class object (AsyncResult).
         */
        void setAsyncResult(GSheetAsyncResult &aResult)
        {
            refResult = &aResult;
            auth_data.refResult = &aResult;
        }

        /**
         * Set the UID for authentication task.
         *
         * @param uid The unique identifier for the authentication task.
         *
         * The UID will be applied when used with the async result callback only.
         * The async result object set via initializeApp and GSheetApp::setAsyncResult will not take effect.
         */
        void setUID(const String &uid) { this->uid = uid; }

#if defined(GSHEET_ENABLE_JWT)
        /**
         * Set the JWT token processor object.
         *
         * This function should be executed before calling initializeApp.
         *
         * @param jwtClass The JWT token processor object.
         *
         */
        void setJWTProcessor(GSheetJWTClass &jwtClass) { this->jwtClass = &jwtClass; }
#endif

        /**
         * Get the pointer to the internal auth data.
         *
         * @return auth_data_t* The pointer to internal auth data.
         */
        auth_data_t *getAuth() { return &auth_data; }
    };
};

#endif