#pragma once
#include <fstream>
#include <string>
#include <curl/curl.h>
#include<zip.h>
#include <sys/stat.h>
#include <tiny/tdf.h>
#include <json.h>
#ifndef GAMEPLATFORM
#define GAMEPLATFORM "l5.64"
#endif
extern size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
inline void makedirs(std::string path) {
	size_t pos = 0;
	while ((pos = path.find_first_of('/', pos)) != std::string::npos) {
		std::string dir = path.substr(0, pos);
		if (!dir.empty()) {
#ifdef _WIN32
			mkdir(dir.c_str(), 0);
#else
			mkdir(dir.c_str(), 0755);
#endif
		}
		pos++;
	}
}
struct netio {
	netio()= default;
	std::string url="https://github.com/kin-fuyuki/forespend_cpp/releases/download/";
	std::vector<std::string> *versionhistory;
	std::string changelog="could not connect to github\nplease check your internet connection";
	std::string currentversion="NULL";
	std::string newestversion="";

	void start() {
    const int MAX_RETRIES = 3;
    int attempts = 0;
    bool download_success = false;

    while (attempts < MAX_RETRIES && !download_success) {
        CURL* curltdf = curl_easy_init();
        if (curltdf) {
            FILE* history = fopen("versions.tdf", "wb");
            if (!history) break;

            curl_easy_setopt(curltdf, CURLOPT_URL, "https://raw.githubusercontent.com/kin-fuyuki/allgames/refs/heads/master/versions.tdf");
            curl_easy_setopt(curltdf, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curltdf, CURLOPT_WRITEDATA, history);
            
            CURLcode ret = curl_easy_perform(curltdf);
            fclose(history);
            curl_easy_cleanup(curltdf);

            if (ret == CURLE_OK) {
                download_success = true;
            } else {
                attempts++;
                tiny::warning("Version fetch failed (Attempt %d/%d): %s", attempts, MAX_RETRIES, curl_easy_strerror(ret));
            }
        }
    }

    if (!download_success) {
        tiny::fatal("Could not download versions.tdf after %d attempts.", MAX_RETRIES);
        return;
    }

    // Processing the file
    tiny::TDF_FILE fetchversions;
    fetchversions.filepath = "versions.tdf";
    fetchversions.read();
    
    boost::unordered_map<std::string, tiny::TDF_DATA>* foreversions = fetchversions.getclass({"forespend"});
    if (!foreversions) return;

    std::vector<std::string> versions;
    for (auto& version : *foreversions) {
        if (version.second.type == tiny::TDF_Type::TDF_DEFINES) {
            versions = *(std::vector<std::string>*)version.second.datapointer;
        }
    }

    if (versions.empty()) return;

    std::sort(versions.begin(), versions.end(), [](const std::string& a, const std::string& b) {
        int ra = 0, rb = 0;
        float va = 0.0f, vb = 0.0f;
        char pa = ' ', pb = ' ';
        char rea = ' ', reb = ' ';
        sscanf(a.c_str(), "%d.%f.%c.%c", &ra, &va, &pa, &rea);
        sscanf(b.c_str(), "%d.%f.%c.%c", &rb, &vb, &pb, &reb);
        if (ra != rb) return ra > rb;
        if (va != vb) return va > vb;
        if (pa != pb) return pa > pb;
        return rea < reb;
    });

    this->newestversion = versions[0]; // Sort is descending, index 0 is newest
    if (this->newestversion != "") {
        this->currentversion = newestversion;
    }

    tiny::success("\n\nforespend versions found:");
    for (auto& version : versions) {
        tiny::warning("%s", version.c_str());
    }

    // Use a member or persistent copy to avoid pointer to local variable
    static std::vector<std::string> persistent_versions;
    persistent_versions = versions;
    versionhistory = &persistent_versions;
}
	void github() {
		CURL* curl;
		CURLcode res;
		FILE* commitsjson = fopen("commitsjson", "wb");
		curl = curl_easy_init();
		if (curl) {
			std::string u = "https://api.github.com/repos/kin-fuyuki/allgames/commits";
			struct curl_slist* headers = NULL;
			headers = curl_slist_append(headers, "User-Agent: C++-App"); // Required by GitHub
			curl_easy_setopt(curl, CURLOPT_URL, u.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, commitsjson);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);
			res = curl_easy_perform(curl);
			fclose(commitsjson);
			changelog="changelog download error";
			if (res == CURLE_OK) {
			try {
			changelog="json read error";
				FILE* thecommits=fopen("commitsjson","rb");
			changelog="json parse error";
				nlohmann::json commits = nlohmann::json::parse(thecommits);
				std::ostringstream oss;
				std::vector<std::string> monthNames = {
					"", "jan", "feb", "mar", "apr", "may", "jun", 
					"jul", "aug", "sep", "oct", "nov", "dec"
				};
				std::string lastMonth = "";
				std::string currentYear = "";
				int limit = std::min((int)commits.size(), 8);
				for (int i = 0; i < limit; ++i) {
					auto& item = commits[i];
					std::string message = item["commit"]["message"];
					std::string date    = item["commit"]["author"]["date"]; // Format: 2025-12-21T13:08:50Z
					// Extract components: YYYY-MM-DD and HH:MM:SS
					std::string y = date.substr(0, 4);
					std::string m = date.substr(5, 2);
					std::string d = date.substr(8, 2);
					std::string t = date.substr(11, 8);
					// Format: MM/DD/YYYY HH:MM:SS
					std::string first_line = message.substr(0, message.find('\n'));
					oss << m << "/" << d << "/" << y << " " << t << "\n" << "    " << first_line << "\n";
				}
				changelog = oss.str();
				tiny::fatal("%s", changelog.c_str() );
			} catch (nlohmann::json::parse_error& e) {
				tiny::fatal("JSON Parse Error: %s", e.what());
			}
		}
			
		}
	}
	void download() {
		CURLcode ret;
			std::string version=versionhistory->at(0);
			tiny::success("using version: %s\ndownloading from: %s",version.c_str(),(url+version+"/"+GAMEPLATFORM+"."+version+".zip").c_str());
			
			CURL*curl = curl_easy_init();
			FILE* file = fopen(("fore."+version+".zip").c_str(), "wb");
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);
			curl_easy_setopt(curl, CURLOPT_URL, "https://github.com/kin-fuyuki/forespend_cpp/releases/download/0.03.a.g/l5.64.0.03.a.g.zip");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
			ret = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			fclose(file);
			
			
			makedirs("forespend/versions/"+version+"/");
			int err=0;
			zip *z=zip_open("fore.0.03.a.g.zip",0,&err);
			if (z) {
				
				zip_int64_t it=zip_get_num_entries(z, 0);
				for (zip_int64_t i = 0; i < it; ++i) {
					struct zip_stat st;
					zip_stat_init(&st);
					if (zip_stat_index(z, i, 0, &st) != 0) continue;
					std::string name = st.name;
					std::string full = "forespend/versions/"+version+"/" + name;
					if (name.back() == '/') {
#ifdef _WIN32
						mkdir(full.c_str(), 0);
#else
						mkdir(full.c_str(), 0755);
#endif
						continue;
					}
					size_t pos = full.find_last_of('/');
					if (pos != std::string::npos) {
						std::string dir = full.substr(0, pos);
#ifdef _WIN32
						mkdir(dir.c_str(), 0);
#else
						mkdir(dir.c_str(), 0755);
#endif
					}
					zip_file_t *zf = zip_fopen_index(z, i, 0);
					if (zf) {
						std::ofstream ofs(full, std::ios::binary);
						char buff[1 << 16];
						zip_int64_t read;
						while ((read = zip_fread(zf, buff, sizeof(buff))) > 0) {
							ofs.write(buff, read);
						}
						ofs.close();
						zip_fclose(zf);
					}
				}
				zip_close(z);
			}
		}
	
};