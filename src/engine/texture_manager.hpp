
#pragma once

struct lnx_tex2d {
    unsigned int id;
    int width, height;
    int nrch;

    const char *texPath;
};

class texture_manager {

    public:
        texture_manager() { }

        void gen_texture(lnx_tex2d &tex);

    private:

};
