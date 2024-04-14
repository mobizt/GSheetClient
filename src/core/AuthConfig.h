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
#ifndef GSHEET_CORE_AUTH_CONFIG_H
#define GSHEET_CORE_AUTH_CONFIG_H
#include <Arduino.h>
#include "./core/FileConfig.h"
#include "./core/StringUtil.h"

#include "./core/AsyncResult/AsyncResult.h"

#define GSHEET_DEFAULT_TOKEN_TTL 3300

namespace gsheet_sa_ns
{
    enum data_item_type_t
    {
        cm,   // client email
        pid,  // project Id
        pk,   // private key
        pkid, // private key Id
        cid,  // client Id
        max_type
    };
}

namespace gsheet_access_tk_ns
{
    enum data_item_type_t
    {
        token,   // access token
        refresh, // refresh token
        cid,     // client id
        csec,    // client secret
        max_type
    };
}

namespace gsheet_app_tk_ns
{
    enum data_item_type_t
    {
        type,    // token type
        token,   // auth token
        refresh, // refresh token
        uid,     // UID
        pid,     // project Id
        max_type
    };
}

namespace gsheet
{
    enum core_auth_task_type
    {
        core_auth_task_type_undefined = -1,
        core_auth_task_type_authenticate,
        core_auth_task_type_refresh_token
    };

    enum auth_event_type
    {
        auth_event_uninitialized = 0,
        auth_event_initializing = 1,
        auth_event_token_signing = 2,
        auth_event_authenticating = 3,
        auth_event_auth_request_sent = 4,
        auth_event_auth_response_received = 5,
        auth_event_ready = 6,
        auth_event_error = 7
    };

    enum jwt_step
    {
        jwt_step_begin,
        jwt_step_sign,
        jwt_step_ready,
        jwt_step_error
    };

    enum auth_token_type
    {
        auth_unknown_token,
        auth_sa_access_token,
        auth_access_token,
        auth_refresh_token
    };

    enum user_auth_data_type
    {
        user_auth_data_undefined,
        user_auth_data_service_account,
        user_auth_data_access_token
    };

    struct auth_status
    {
        friend class user_auth_data;
        friend class GSheetApp;

    private:
        auth_event_type _event = auth_event_uninitialized;

        String authEventString(auth_event_type ev)
        {
            String evstr;
            switch (ev)
            {
            case auth_event_initializing:
                evstr = FPSTR("initializing");
                break;
            case auth_event_token_signing:
                evstr = FPSTR("token signing");
                break;
            case auth_event_authenticating:
                evstr = FPSTR("authenticating");
                break;
            case auth_event_auth_request_sent:
                evstr = FPSTR("auth request sent");
                break;
            case auth_event_auth_response_received:
                evstr = FPSTR("auth response received");
                break;
            case auth_event_ready:
                evstr = FPSTR("ready");
                break;
            case auth_event_error:
                evstr = FPSTR("error");
                break;
            default:
                evstr = FPSTR("undefined");
                break;
            }

            return evstr;
        }

    public:
        auth_event_type event() const { return _event; }
    };

    typedef void (*GSheetTimeStatusCallback)(uint32_t &ts);

    struct user_auth_data
    {
        friend class GSheetSAParser;
        friend class GSheetServiceAuth;
        friend class GSheetAccessToken;
        friend class GSheetJWTClass;
        friend class GSheetApp;
        friend class GSheetClient;
        friend class GSheetUserTokenFileParser;

#if defined(GSHEET_ENABLE_SERVICE_AUTH)

        struct service_account
        {
            friend class GSheetSAParser;
            friend class GSheetServiceAuth;
            friend class GSheetApp;
            friend class GSheetJWTClass;
            friend class GSheetClient;
            friend class GSheetUserTokenFileParser;

        public:
            service_account() = default;
            ~service_account() { clear(); }
            void copy(service_account &rhs)
            {
                for (size_t i = 0; i < gsheet_sa_ns::max_type; i++)
                    this->val[i] = rhs.val[i];
                this->timestatus_cb = rhs.timestatus_cb;
                this->expire = rhs.expire;
                this->step = rhs.step;
            }

            void clear()
            {
                for (size_t i = 0; i < gsheet_sa_ns::max_type; i++)
                    val[i].remove(0, val[i].length());
                timestatus_cb = NULL;
                expire = GSHEET_DEFAULT_TOKEN_TTL;
                step = jwt_step_begin;
            }

        protected:
            String val[gsheet_sa_ns::max_type];
            jwt_step step = jwt_step_begin;
            GSheetTimeStatusCallback timestatus_cb = NULL;
            size_t expire = GSHEET_DEFAULT_TOKEN_TTL;
        };
#endif

#if defined(GSHEET_ENABLE_ACCESS_TOKEN)
        struct access_token_data
        {
            String val[gsheet_access_tk_ns::max_type];
            size_t expire = GSHEET_DEFAULT_TOKEN_TTL;
            GSheetTimeStatusCallback timestatus_cb = NULL;

        public:
            access_token_data() = default;
            ~access_token_data() { clear(); }
            void copy(access_token_data &rhs)
            {
                for (size_t i = 0; i < gsheet_access_tk_ns::max_type; i++)
                    this->val[i] = rhs.val[i];
                this->expire = rhs.expire;
                this->timestatus_cb = rhs.timestatus_cb;
            }
            void clear()
            {
                for (size_t i = 0; i < gsheet_access_tk_ns::max_type; i++)
                    val[i].remove(0, val[i].length());
                expire = GSHEET_DEFAULT_TOKEN_TTL;
                timestatus_cb = NULL;
            }
        };
#endif

    public:
        user_auth_data() = default;
        ~user_auth_data() { clear(); }
        user_auth_data &operator=(user_auth_data &rhs)
        {
            copy(rhs);
            return *this;
        }
        void copy(user_auth_data &rhs)
        {
#if defined(GSHEET_ENABLE_SERVICE_AUTH)
            this->sa.copy(rhs.sa);
#endif

#if defined(GSHEET_ENABLE_ACCESS_TOKEN)
            this->access_token.copy(rhs.access_token);
#endif

            this->auth_type = rhs.auth_type;
            this->auth_data_type = rhs.auth_data_type;
            this->anonymous = rhs.anonymous;
            this->initialized = rhs.initialized;
            this->timestatus_cb = rhs.timestatus_cb;
        }

        void clear()
        {

#if defined(GSHEET_ENABLE_SERVICE_AUTH)
            sa.clear();
#endif

#if defined(GSHEET_ENABLE_ACCESS_TOKEN)
            access_token.clear();
#endif

            timestatus_cb = NULL;
            anonymous = false;
            initialized = false;
        }

    protected:
#if defined(GSHEET_ENABLE_SERVICE_AUTH)
        service_account sa;
#endif

#if defined(GSHEET_ENABLE_ACCESS_TOKEN)
        access_token_data access_token;
#endif

        bool anonymous = false;
        bool initialized = false;
        bool jwt_signing = false;
        bool jwt_loop = false;
        uint32_t jwt_ts = 0;
        auth_token_type auth_type = auth_unknown_token;
        user_auth_data_type auth_data_type = user_auth_data_undefined;
        core_auth_task_type task_type = core_auth_task_type_undefined;
        auth_status status;
        GSheetTimeStatusCallback timestatus_cb = NULL;
    };

#if defined(GSHEET_ENABLE_FS)

#if defined(GSHEET_ENABLE_ACCESS_TOKEN)

    class GSheetUserTokenFileParser
    {
    public:
        enum token_type
        {
            token_type_user_data,
            token_type_id_token,
            token_type_access_token,
            token_type_custom_token,
            token_type_legacy_token
        };

        static bool parseUserFile(token_type type, FILEOBJ userfile, user_auth_data &auth_data)
        {
            String buff;

            if (userfile && userfile.size())
            {
                while (userfile.available())
                {
                    buff += (char)userfile.read();
                }

                GSheetMemory mem;
                GSheetStringUtil sut;
                int tokenSize = 1;
                for (size_t i = 0; i < buff.length(); i++)
                {
                    if (buff[i] == ',')
                        tokenSize++;
                }

                String tokens[tokenSize];
                char *p = reinterpret_cast<char *>(mem.alloc(buff.length()));
                strcpy(p, buff.c_str());
                char *pp = p;
                char *end = p;
                String tmp;
                int i = 0;
                while (pp != NULL)
                {
                    sut.strsepImpl(&end, ",");
                    if (strlen(pp) > 0)
                    {
                        tmp = pp;
                        tokens[i] = tmp;
                        i++;
                    }
                    pp = end;
                }
                // release memory
                mem.release(&p);

                if (type == token_type_access_token && tokenSize == 5)
                {
#if defined(GSHEET_ENABLE_ACCESS_TOKEN)
                    for (size_t i = 0; i < gsheet_access_tk_ns::max_type; i++)
                        auth_data.access_token.val[i] = tokens[i];
                    auth_data.access_token.expire = atoi(tokens[4].c_str());
#endif
                    return true;
                }
            }

            return false;
        }

        static bool saveUserFile(token_type type, GSHEET_FILEOBJ userfile, user_auth_data &auth_data)
        {
            String buff;

            if (userfile)
            {
                if (type == token_type_access_token)
                {
#if defined(GSHEET_ENABLE_ACCESS_TOKEN)
                    for (size_t i = 0; i < gsheet_access_tk_ns::max_type; i++)
                    {
                        userfile.print(auth_data.access_token.val[i].c_str());
                        userfile.print(FPSTR(","));
                    }
                    userfile.print(String(auth_data.access_token.expire).c_str());
#endif
                    return true;
                }
            }
            return false;
        }
    };

#endif

#if defined(GSHEET_ENABLE_SERVICE_AUTH)
    class GSheetSAParser
    {
    public:
        static bool parseSAFile(FILEOBJ safile, user_auth_data &auth_data)
        {
            bool ret = false;

            if (safile && safile.size())
            {

                String buf;
                buf.reserve(safile.size() + 10);
                while (safile.available())
                {
                    buf += (char)safile.read();
                }

                safile.close();

                GSheetStringUtil sut;
                int p1 = 0, p2 = 0;
                sut.parse(buf, "\"project_id\"", ",", p1, p2);
                if (p1 > -1 && p2 > -1)
                {
                    auth_data.sa.val[gsheet_sa_ns::pid] = buf.substring(p1 + 1, p2 - 1);
                    p1 = p2;
                }
                sut.parse(buf, "\"private_key_id\"", ",", p1, p2);
                if (p1 > -1 && p2 > -1)
                {
                    auth_data.sa.val[gsheet_sa_ns::pkid] = buf.substring(p1 + 1, p2 - 1);
                    p1 = p2;
                }
                sut.parse(buf, "\"private_key\"", ",", p1, p2);
                if (p1 > -1 && p2 > -1)
                {
                    auth_data.sa.val[gsheet_sa_ns::pk] = buf.substring(p1 + 1, p2 - 1);
                    p1 = p2;
                    auth_data.initialized = true;
                    ret = true;
                }
                sut.parse(buf, "\"client_email\"", ",", p1, p2);
                if (p1 > -1 && p2 > -1)
                {
                    auth_data.sa.val[gsheet_sa_ns::cm] = buf.substring(p1 + 1, p2 - 1);
                    p1 = p2;
                }
                sut.parse(buf, "\"client_id\"", ",", p1, p2);
                if (p1 > -1 && p2 > -1)
                {
                    auth_data.sa.val[gsheet_sa_ns::cid] = buf.substring(p1 + 1, p2 - 1);
                    p1 = p2;
                }
            }

            return ret;
        }
    };

#endif

#endif

#if defined(GSHEET_ENABLE_SERVICE_AUTH)
    class GSheetServiceAuth
    {
        friend class FirebaseApp;

    public:
        GSheetServiceAuth(GSheetTimeStatusCallback timeCb, const String &clientEmail, const String &projectId, const String &privateKey, size_t expire = GSHEET_DEFAULT_TOKEN_TTL)
        {
            data.clear();
            data.sa.val[gsheet_sa_ns::cm] = clientEmail;
            data.sa.val[gsheet_sa_ns::pid] = projectId;
            data.sa.val[gsheet_sa_ns::pk] = privateKey;
            data.sa.expire = expire;
            data.initialized = isInitialized();
            data.auth_type = auth_sa_access_token;
            data.auth_data_type = user_auth_data_service_account;
            data.timestatus_cb = timeCb;
        };

        GSheetServiceAuth(GSheetTimeStatusCallback timeCb, gsheet_file_config_data &safile)
        {
#if defined(GSHEET_ENABLE_FS)
            data.clear();
            if (safile.initialized)
            {
                safile.cb(safile.file, safile.filename.c_str(), gsheet_file_mode_open_read);
                if (safile.file)
                {
                    GSheetSAParser::parseSAFile(safile.file, data);
                    safile.file.close();
                }
                data.timestatus_cb = timeCb;
            }
#endif
        }

        ~GSheetServiceAuth() { data.clear(); };
        void clear() { data.clear(); }
        user_auth_data &get() { return data; }
        bool isInitialized() { return data.sa.val[gsheet_sa_ns::cm].length() > 0 && data.sa.val[gsheet_sa_ns::pid].length() > 0 && data.sa.val[gsheet_sa_ns::pk].length() > 0; }

    private:
        user_auth_data data;
    };

#endif

#if defined(GSHEET_ENABLE_ACCESS_TOKEN)

    class GSheetAccessToken
    {
        friend class GSheetApp;

    public:
        GSheetAccessToken(const String &token, size_t expire = GSHEET_DEFAULT_TOKEN_TTL, const String &refresh = "", const String &client_id = "", const String &client_secret = "")
        {
            this->data.clear();
            this->data.access_token.val[gsheet_access_tk_ns::token] = token;
            this->data.access_token.val[gsheet_access_tk_ns::refresh] = refresh;
            this->data.access_token.val[gsheet_access_tk_ns::cid] = client_id;
            this->data.access_token.val[gsheet_access_tk_ns::csec] = client_secret;
            this->data.access_token.expire = expire;
            this->data.initialized = true;
            this->data.auth_type = auth_access_token;
            this->data.auth_data_type = user_auth_data_access_token;
        }

        GSheetAccessToken(gsheet_file_config_data &tokenFile)
        {
#if defined(GSHEET_ENABLE_FS)
            data.clear();
            if (tokenFile.initialized)
            {
                tokenFile.cb(tokenFile.file, tokenFile.filename.c_str(), gsheet_file_mode_open_read);
                if (tokenFile.file)
                {
                    if (GSheetUserTokenFileParser::parseUserFile(GSheetUserTokenFileParser::token_type_access_token, tokenFile.file, data))
                    {
                        data.initialized = true;
                        data.auth_type = auth_access_token;
                        data.auth_data_type = user_auth_data_access_token;
                    }
                    tokenFile.file.close();
                }
            }
#endif
        }

        bool save(gsheet_file_config_data &tokenFile)
        {
            bool ret = false;
#if defined(GSHEET_ENABLE_FS)
            if (tokenFile.initialized)
            {
                tokenFile.cb(tokenFile.file, tokenFile.filename.c_str(), gsheet_file_mode_open_write);
                if (tokenFile.file)
                {
                    ret = GSheetUserTokenFileParser::saveUserFile(GSheetUserTokenFileParser::token_type_access_token, tokenFile.file, data);
                    tokenFile.file.close();
                }
            }
#endif
            return ret;
        }

        void clear() { data.clear(); }
        user_auth_data &get() { return data; }

    private:
        user_auth_data data;
    };

#endif

    struct app_token_t
    {
    public:
        String val[gsheet_app_tk_ns::max_type];
        uint32_t expire = 0;
        bool authenticated = false;
        auth_token_type auth_type = auth_unknown_token;
        user_auth_data_type auth_data_type = user_auth_data_undefined;
        void clear()
        {
            for (size_t i = 0; i < gsheet_app_tk_ns::max_type; i++)
                val[i].remove(0, val[i].length());
            expire = GSHEET_DEFAULT_TOKEN_TTL;
            authenticated = false;
            auth_type = auth_unknown_token;
            auth_data_type = user_auth_data_undefined;
        }
        app_token_t() = default;
    };

    struct auth_data_t
    {
        user_auth_data user_auth;
        app_token_t app_token;
        GSheetAsyncResultCallback cb;
    };

};

#endif