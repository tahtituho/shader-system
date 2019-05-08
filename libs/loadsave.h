#pragma once

#include <emgui/Types.h>

struct TrackData;
typedef wchar_t text_t;
int LoadSave_loadRocketXML(const text_t* path, struct TrackData* trackData);
int LoadSave_loadRocketXMLDialog(text_t* path, struct TrackData* trackData);
int LoadSave_saveRocketXML(const text_t* path, struct TrackData* trackData);
int LoadSave_saveRocketXMLDialog(text_t* path, struct TrackData* trackData);
