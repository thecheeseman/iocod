#ifndef LOAD_FOLDER_DATA_H
#define LOAD_FOLDER_DATA_H

enum folder_data_type {
    FDT_NONE,
    FDT_ANIMTREES,
    FDT_XANIM,
    FDT_XMODEL,
    FDT_XMODELPARTS,
    FDT_XMODELSURFS,
    FDT_WEAPONS,
    FDT_END
};

void fs_load_folder_data(enum folder_data_type folder);

#endif /* LOAD_FOLDER_DATA_H */
