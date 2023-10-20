#include "update.hpp"

#include <vector>
#include <fstream>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <zip.h>

#include "core/globals.hpp"
#include "core/logging.hpp"
#include "core/utils.hpp"
#include "commit_hash.h"

bool update::check_for_updates()
{
    logging::log("[hi-octane::init] Checking for updates...");
    std::string read_buffer{};
    CURL* curl = curl_easy_init();
    if (curl != nullptr) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/itsmeft24/hi-octane-hook/commits/main");

        curl_slist* list = curl_slist_append(nullptr, "User-Agent: Cars-Hi-Octane");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::curl_string_write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            rapidjson::Document doc;
            doc.Parse<rapidjson::kParseStopWhenDoneFlag>(read_buffer.data());
            std::string current_commit_hash = std::format("{:x}", COMMIT_HASH);
            std::string latest_commit_hash = doc.GetObject()["sha"].GetString();
            if (latest_commit_hash.substr(7 - current_commit_hash.size(), current_commit_hash.size()) == current_commit_hash) {
                logging::log("[hi-octane::init] No updates were found. You're all up-to-date!");
                curl_easy_cleanup(curl);
                return false;
            }
            else {
                curl_easy_cleanup(curl);
                return true;
            }
        }
        else {
            logging::log("[hi-octane::init] Failed to check for updates. Continuing startup...");
        }
        curl_easy_cleanup(curl);
    }
    return false;
}

bool update::download_latest_release()
{
    std::vector<std::uint8_t> release_zip{};

    CURL* curl = curl_easy_init();
    if (curl != nullptr) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://github.com/itsmeft24/hi-octane-hook/releases/download/beta/release.zip");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::curl_vec_write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &release_zip);
        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            zip_error_t error{};
            zip_source_t* zs = zip_source_buffer_create(release_zip.data(), release_zip.size(), 0, &error);
            zip_t* zip = zip_open_from_source(zs, ZIP_RDONLY, &error);

            zip_file_t* dll = zip_fopen(zip, "CarsGDF.dll", 0);
            zip_stat_t st{};
            zip_stat_init(&st);
            zip_stat(zip, "CarsGDF.dll", 0, &st);
            std::vector<std::uint8_t> dllbytes(st.size);
            zip_fread(dll, dllbytes.data(), st.size);
            zip_fclose(dll);

            zip_file_t* lib = zip_fopen(zip, "CarsGDF.lib", 0);
            zip_stat_init(&st);
            zip_stat(zip, "CarsGDF.lib", 0, &st);
            std::vector<std::uint8_t> libbytes(st.size);
            zip_fread(lib, libbytes.data(), st.size);
            zip_fclose(lib);

            zip_close(zip);

            std::filesystem::rename(g_InstallDir / "CarsGDF.dll", g_InstallDir / "CarsGDF.dll.old");
            std::filesystem::rename(g_InstallDir / "CarsGDF.lib", g_InstallDir / "CarsGDF.lib.old");

            std::ofstream outdll(g_InstallDir / "CarsGDF.dll", std::ios::out | std::ios::binary);
            outdll.write(reinterpret_cast<char*>(dllbytes.data()), dllbytes.size());

            std::ofstream outlib(g_InstallDir / "CarsGDF.lib", std::ios::out | std::ios::binary);
            outlib.write(reinterpret_cast<char*>(libbytes.data()), libbytes.size());
            logging::log("[hi-octane::init] Downloaded latest release. Please restart the game!");
            curl_easy_cleanup(curl);
            return true;
        }
        else {
            logging::log("[hi-octane::init] Failed to download release. Continuing startup...");
            curl_easy_cleanup(curl);
            return false;
        }
    }
    return false;
}

void update::delete_temporary_files()
{
    if (std::filesystem::exists(g_InstallDir / "CarsGDF.dll.old")) {
        std::filesystem::remove(g_InstallDir / "CarsGDF.dll.old");
    }
    if (std::filesystem::exists(g_InstallDir / "CarsGDF.lib.old")) {
        std::filesystem::remove(g_InstallDir / "CarsGDF.lib.old");
    }
}
