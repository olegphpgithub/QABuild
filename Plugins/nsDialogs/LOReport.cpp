
#include "Windows.h"
#include <string>
#include "nsis/pluginapi.h"
#include "wininet.h"



TCHAR param[1024];
struct SReportParam
{
    std::string url;
    int begin_feedback;
    int timeout;
    int count;
};


HANDLE g_hLOReportThread;
bool g_StopSending;


void sendRequest(char *url)
{
    DWORD g_stringsize = 1024;
    char host[1024];
    char user[1024];
    char passwd[1024];
    char path[1024];
    char params[1024];

	prepareURI(url);

    URL_COMPONENTS uc = {sizeof(URL_COMPONENTS), NULL, 0,
        (INTERNET_SCHEME)0, host, g_stringsize, 0 , user, g_stringsize,
        passwd, g_stringsize, path, g_stringsize, params, g_stringsize};

    
    if(InternetCrackUrl(url, 0, 0/*ICU_ESCAPE*/ , &uc))
    {
        std::string new_path = path;
        //new_path+= "?";
        new_path+= params;

        char httpUseragent[] = "NSIS_Inetc (Mozilla)";
        char Method[5] =  "GET\0";
        
        HINTERNET internet = InternetOpen(httpUseragent, 0, NULL, NULL, 0);
        if(internet != NULL)
        {
            HINTERNET connect = InternetConnect(internet, host, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
            if(connect != NULL)
            {
                HINTERNET request = HttpOpenRequest(connect, Method, new_path.c_str(), "HTTP/1.1", NULL, NULL, 
                    INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD|INTERNET_FLAG_SECURE, NULL);
                if(request != NULL)
                {
                    
                    HttpSendRequest(request, 0, 0, 0, 0);

                    InternetCloseHandle(request);
                }
            }
            InternetCloseHandle(connect);
        }
        InternetCloseHandle(internet);
    }
}

DWORD ReportThread(LPVOID lpParameter)
{
    char url[1024];
    SReportParam *sparam = (SReportParam*)lpParameter;
    if (sparam == NULL)
    {
        return 0;
    }

    for(int i = 0; i<sparam->count; i++)
    {
        if (g_StopSending == true) break;
        sprintf_s(url,"%s%i",sparam->url.c_str(),sparam->begin_feedback+i);
        // send feedback 
        sendRequest(url);
        // sleep
        for (int j = 0 ; j < sparam->timeout*10; j++)
        {
            if (g_StopSending == true) break;
            Sleep(100);
        }
    }
    return 0;
}

Function(Report)
{
    EXDLL_INIT();
    {

        SReportParam *sparam = new SReportParam;
        popstring(param);
        int code = myatoi(param);
        if (code == 0)
        {
            popstring(param);
            sparam->url  = param;
            popstring(param);
            sparam->begin_feedback  = myatoi(param);
            popstring(param);
            sparam->count  = myatoi(param);
            popstring(param);
            sparam->timeout  = myatoi(param);
            DWORD dwId;
            g_StopSending = false;
            g_hLOReportThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ReportThread, sparam, 0, &dwId);
            return;
        }
        if (code == 1)
        {
            g_StopSending = true;
        }
    }
}