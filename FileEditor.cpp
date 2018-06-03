//
//  FileEditor.cpp
//  SLUG
//
//  Created by Steph on 3/17/16.
//
//

#include "FileEditor.h"
#include <string>

char *data_path = NULL;

void InitializeDataPath() {
    char *base_path = SDL_GetBasePath();
    if (base_path) {
        data_path = base_path;
    } else {
        data_path = SDL_strdup("./");
    }
}

char* file_read(const char* filename) {
    if (data_path == NULL) {
        InitializeDataPath();
    }
    std::string name = std::string(data_path) + filename;
    SDL_RWops *rw = SDL_RWFromFile(name.c_str(), "rb");
    if (rw == NULL) return NULL;
    
    Sint64 res_size = SDL_RWsize(rw);
    char* res = (char*)malloc(res_size + 1);
    
    Sint64 nb_read_total = 0, nb_read = 1;
    char* buf = res;
    while (nb_read_total < res_size && nb_read != 0) {
        nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
        nb_read_total += nb_read;
        buf += nb_read;
    }
    SDL_RWclose(rw);
    if (nb_read_total != res_size) {
        free(res);
        return NULL;
    }
    
    res[nb_read_total] = '\0';
    return res;
}