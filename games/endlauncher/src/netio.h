#pragma once
#include <fstream>
#include <string>
#include <curl/curl.h>
#include<zip.h>
#include <sys/stat.h>
#include <tiny/tdf.h>
extern size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

struct netio {
	netio()= default;
	std::string url="https://github.com/kin-fuyuki/forespend_cpp/releases/download/";
	std::vector<std::string> *versionhistory;
	void download() {
		CURL* curl = curl_easy_init();
		CURLcode ret;
		if (curl) {
			FILE* history = fopen("versions.tdf", "wb");
			curl_easy_setopt(curl, CURLOPT_URL, "https://raw.githubusercontent.com/kin-fuyuki/allgames/refs/heads/master/versions.tdf");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, history);
			ret = curl_easy_perform(curl);
			
			tiny::TDF_FILE fetchversions;
			fetchversions.filepath = "versions.tdf";
			fetchversions.read();
			boost::unordered_map<std::string,tiny::TDF_DATA>* foreversions= fetchversions.getclass({"forespend"});
			std::vector<std::string> versions;
			for (auto version:*foreversions) {
				if (version.second.type==tiny::TDF_Type::TDF_DEFINES) {
					versions=*(std::vector<std::string>*)version.second.datapointer;
				}
			}
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
#ifndef GAMEPLATFORM
#define GAMEPLATFORM "l5.64"
#endif
			versionhistory=&versions;
			std::string version=versionhistory->at(0);
			FILE* file = fopen(("fore."+version+".zip").c_str(), "wb");
			curl_easy_setopt(curl, CURLOPT_URL, (url+version+"/"+GAMEPLATFORM+"."+version+".zip").c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
			ret = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			fclose(file);

			
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
	}
	
};