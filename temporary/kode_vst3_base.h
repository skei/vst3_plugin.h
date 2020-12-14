#ifndef kode_vst3_base_included
#define kode_vst3_base_included
//----------------------------------------------------------------------
// based on (extracted from) steinberg vst3 sdk (gpl3)

//----------------------------------------------------------------------

#ifdef __gnu_linux__
  #define VST3_COM_COMPATIBLE 0
  #define VST3_PLUGIN_API
#endif

//#ifdef _WIN32
//  #define VST3_COM_COMPATIBLE 1
//  #define VST3_PLUGIN_API __stdcall
//#endif

//----------

#include <string>
#include <locale>
#include <codecvt>

//----------

#define VST3_PARAM_AFTERTOUCH  0x10000    // kAfterTouch (128)
#define VST3_PARAM_PITCHBEND   0x20000    // kPitchBend (129)
#define VST3_PARAM_BRIGHTNESS  0x30000    // kCtrlFilterResonance (74)

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#if VST3_COM_COMPATIBLE
  #define VST3_INLINE_UID(l1,l2,l3,l4) {                                  \
    (int8_t)((l1 & 0x000000FF)      ), (int8_t)((l1 & 0x0000FF00) >>  8), \
    (int8_t)((l1 & 0x00FF0000) >> 16), (int8_t)((l1 & 0xFF000000) >> 24), \
    (int8_t)((l2 & 0x00FF0000) >> 16), (int8_t)((l2 & 0xFF000000) >> 24), \
    (int8_t)((l2 & 0x000000FF)      ), (int8_t)((l2 & 0x0000FF00) >>  8), \
    (int8_t)((l3 & 0xFF000000) >> 24), (int8_t)((l3 & 0x00FF0000) >> 16), \
    (int8_t)((l3 & 0x0000FF00) >>  8), (int8_t)((l3 & 0x000000FF)      ), \
    (int8_t)((l4 & 0xFF000000) >> 24), (int8_t)((l4 & 0x00FF0000) >> 16), \
    (int8_t)((l4 & 0x0000FF00) >>  8), (int8_t)((l4 & 0x000000FF)      )  \
  }
#else
  #define VST3_INLINE_UID(l1, l2, l3, l4) {                               \
    (int8_t)((l1 & 0xFF000000) >> 24), (int8_t)((l1 & 0x00FF0000) >> 16), \
    (int8_t)((l1 & 0x0000FF00) >>  8), (int8_t)((l1 & 0x000000FF)      ), \
    (int8_t)((l2 & 0xFF000000) >> 24), (int8_t)((l2 & 0x00FF0000) >> 16), \
    (int8_t)((l2 & 0x0000FF00) >>  8), (int8_t)((l2 & 0x000000FF)      ), \
    (int8_t)((l3 & 0xFF000000) >> 24), (int8_t)((l3 & 0x00FF0000) >> 16), \
    (int8_t)((l3 & 0x0000FF00) >>  8), (int8_t)((l3 & 0x000000FF)      ), \
    (int8_t)((l4 & 0xFF000000) >> 24), (int8_t)((l4 & 0x00FF0000) >> 16), \
    (int8_t)((l4 & 0x0000FF00) >>  8), (int8_t)((l4 & 0x000000FF)      )  \
  }
#endif

//----------

#define VST3_DECLARE_CLASS_IID(classname,l1,l2,l3,l4) \
  static const vst3_iid classname##_iid = VST3_INLINE_UID(l1,l2,l3,l4);

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

typedef char16_t  vst3_utf16str[128];
typedef char      vst3_iid[16];

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#if VST3_COM_COMPATIBLE

  #ifdef _WIN32

    enum {
      kNoInterface		  = static_cast<int32_t>(0x80004002L),	// E_NOINTERFACE
      kResultOk         = static_cast<int32_t>(0x00000000L),	// S_OK
      kResultTrue       = kResultOk,
      kResultFalse		  = static_cast<int32_t>(0x00000001L),	// S_FALSE
      kInvalidArgument	= static_cast<int32_t>(0x80070057L),	// E_INVALIDARG
      kNotImplemented		= static_cast<int32_t>(0x80004001L),	// E_NOTIMPL
      kInternalError		= static_cast<int32_t>(0x80004005L),	// E_FAIL
      kNotInitialized		= static_cast<int32_t>(0x8000FFFFL),	// E_UNEXPECTED
      kOutOfMemory		  = static_cast<int32_t>(0x8007000EL)		// E_OUTOFMEMORY
    };

  #else // _WIN32

    enum {
      kNoInterface      = static_cast<int32_t>(0x80000004L),	// E_NOINTERFACE
      kResultOk         = static_cast<int32_t>(0x00000000L),	// S_OK
      kResultTrue       = kResultOk,
      kResultFalse      = static_cast<int32_t>(0x00000001L),	// S_FALSE
      kInvalidArgument  = static_cast<int32_t>(0x80000003L),	// E_INVALIDARG
      kNotImplemented   = static_cast<int32_t>(0x80000001L),	// E_NOTIMPL
      kInternalError    = static_cast<int32_t>(0x80000008L),	// E_FAIL
      kNotInitialized   = static_cast<int32_t>(0x8000FFFFL),	// E_UNEXPECTED
      kOutOfMemory      = static_cast<int32_t>(0x80000002L)		// E_OUTOFMEMORY
    };

  #endif // _WIN32

#else // VST3_COM_COMPATIBLE

  enum {
    kNoInterface = -1,
    kResultOk,
    kResultTrue = kResultOk,
    kResultFalse,
    kInvalidArgument,
    kNotImplemented,
    kInternalError,
    kNotInitialized,
    kOutOfMemory
  };

#endif // VST3_COM_COMPATIBLE

//----------

enum VST3_NoteExpressionTypeIDs {
  kVolumeTypeID = 0,
  kPanTypeID,
  kTuningTypeID,
  kVibratoTypeID,
  kExpressionTypeID,
  kBrightnessTypeID,
  kTextTypeID,
  kPhonemeTypeID,
  kCustomStart = 100000
};

enum vst3_BusDirections {
  kInput = 0,
  kOutput
};

enum VST3_BusTypes {
  kMain = 0,
  kAux
};

enum VST3_MediaTypes {
  kAudio = 0,
  kEvent,
  kNumMediaTypes
};

enum VST3_SymbolicSampleSizes {
  kSample32,
  kSample64
};

//----------------------------------------------------------------------

const char*     vst3_VstVersionString             = "VST 3.6.10";
const char*     vst3_VstAudioEffectClass          = "Audio Module Class";
const char*     vst3_Editor                       = "editor";
const char*     vst3_Fx					                  = "Fx";
const char*     vst3_Instrument			              = "Instrument";
const char*     vst3_PlatformTypeHWND             = "HWND";
const char*     vst3_PlatformTypeHIView           = "HIView";
const char*     vst3_PlatformTypeNSView           = "NSView";
const char*     vst3_PlatformTypeUIView           = "UIView";
const char*     vst3_PlatformTypeX11EmbedWindowID = "X11EmbedWindowID";
const int32_t   vst3_NoParentUnitId               = -1;
const int32_t   vst3_NoProgramListId              = -1;
const uint32_t  vst3_NoTail                       = 0;
const int32_t   vst3_RootUnitId                   = 0;
const uint64_t  vst3_SpeakerL                     = 1 << 0;
const uint64_t  vst3_SpeakerR                     = 1 << 1;

#define vst3_CtrlFilterResonance  74
#define vst3_AfterTouch           128
#define vst3_PitchBend            129

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class VST3_IParameterChanges;
class VST3_IEventList;

//----------

struct VST3_AudioBusBuffers {
  VST3_AudioBusBuffers() : numChannels(0), silenceFlags(0), channelBuffers64(0) {}
  int32_t   numChannels;
  uint64_t  silenceFlags;
  union {
    float**   channelBuffers32;
    double**  channelBuffers64;
  };
};

struct VST3_BusInfo {
  int32_t       mediaType;
  int32_t       direction;
  int32_t       channelCount;
  vst3_utf16str name;
  int32_t       busType;
  uint32_t    flags;
  enum VST3_BusFlags 	{
    kDefaultActive = 1 << 0
  };
};

struct VST3_Chord {
  uint8_t keyNote;
  uint8_t rootNote;
  int16_t chordMask;
  enum VST3_Masks {
    kChordMask    = 0x0FFF,
    kReservedMask = 0xF000
  };
};

struct VST3_FrameRate {
  enum FrameRateFlags {
    kPullDownRate = 1 << 0,
    kDropRate     = 1 << 1
  };
  uint32_t  framesPerSecond;
  uint32_t  flags;
};

struct VST3_KeyswitchInfo {
  uint32_t      typeId;
  vst3_utf16str title;
  vst3_utf16str shortTitle;
  int32_t       keyswitchMin;
  int32_t       keyswitchMax;
  int32_t       keyRemapped;
  int32_t       unitId;
  int32_t     flags;
};

struct VST3_ParameterInfo {
  uint32_t      id;
  vst3_utf16str title;
  vst3_utf16str shortTitle;
  vst3_utf16str units;
  int32_t       stepCount;
  double        defaultNormalizedValue;
  int32_t       unitId;
  int32_t       flags;
  enum VST3_ParameterFlags {
    kCanAutomate      = 1 << 0,
    kIsReadOnly       = 1 << 1,
    kIsWrapAround     = 1 << 2,
    kIsList           = 1 << 3,
    kIsProgramChange  = 1 << 15,
    kIsBypass         = 1 << 16
  };
};

struct VST3_PClassInfo {
  enum VST3_ClassCardinality {
    kManyInstances = 0x7FFFFFFF
  };
  enum {
    kCategorySize = 32,
    kNameSize = 64
  };
  vst3_iid  cid;
  int32_t   cardinality;
  char      category[kCategorySize];
  char      name[kNameSize];
  VST3_PClassInfo(const vst3_iid _cid, int32_t _cardinality, const char* _category, const char* _name) {
    memset (this, 0, sizeof (VST3_PClassInfo));
    memcpy (cid, _cid, sizeof (vst3_iid));
    if (_category) strncpy(category, _category, kCategorySize);
    if (_name) strncpy(name, _name, kNameSize);
    cardinality = _cardinality;
  }
  //#if SMTG_CPP11
  constexpr VST3_PClassInfo() : cid(), cardinality(), category(), name() {}
  //#else
  //PClassInfo () { memset (this, 0, sizeof (PClassInfo)); }
  //#endif
};

struct VST3_PClassInfo2 {
	vst3_iid  cid;
	int32_t   cardinality;
	char      category[VST3_PClassInfo::kCategorySize];
	char      name[VST3_PClassInfo::kNameSize];
	enum {
		kVendorSize         = 64,
		kVersionSize        = 64,
		kSubCategoriesSize  = 128
	};
	uint32_t  classFlags;
	char      subCategories[kSubCategoriesSize];
	char      vendor[kVendorSize];
	char      version[kVersionSize];
	char      sdkVersion[kVersionSize];
	VST3_PClassInfo2(const vst3_iid _cid, int32_t _cardinality, const char* _category, const char* _name,
		int32_t _classFlags, const char* _subCategories, const char* _vendor, const char* _version,
		const char* _sdkVersion) {
		memset (this, 0, sizeof (VST3_PClassInfo2));
		memcpy (cid, _cid, sizeof (vst3_iid));
		cardinality = _cardinality;
		if (_category) strncpy(category, _category, VST3_PClassInfo::kCategorySize);
		if (_name) strncpy(name, _name, VST3_PClassInfo::kNameSize);
		classFlags = static_cast<uint32_t> (_classFlags);
		if (_subCategories) strncpy(subCategories, _subCategories, kSubCategoriesSize);
		if (_vendor) strncpy(vendor, _vendor, kVendorSize);
		if (_version) strncpy(version, _version, kVersionSize);
		if (_sdkVersion) strncpy(sdkVersion, _sdkVersion, kVersionSize);
	}
//#if SMTG_CPP11
	constexpr VST3_PClassInfo2()
	: cid(), cardinality(), category(), name(), classFlags(), subCategories(), vendor(), version(), sdkVersion() {}
//#else
//	PClassInfo2 () { memset (this, 0, sizeof (PClassInfo2)); }
//#endif
};

/*
struct VST3_PClassInfoW {
	vst3_iid cid;
	int32 cardinality;
	char8 category[PClassInfo::kCategorySize];
	char16 name[PClassInfo::kNameSize];
	enum { kVendorSize = 64,
		kVersionSize = 64,
		kSubCategoriesSize = 128
	};
	uint32 classFlags;
	char8 subCategories[kSubCategoriesSize];
	char16 vendor[kVendorSize];
	char16 version[kVersionSize];
	char16 sdkVersion[kVersionSize];
	PClassInfoW (const vst3_iid _cid, int32 _cardinality, const char8* _category, const char16* _name,
		int32 _classFlags, const char8* _subCategories, const char16* _vendor, const char16* _version,
		const char16* _sdkVersion) {
		memset (this, 0, sizeof (PClassInfoW));
		memcpy (cid, _cid, sizeof (vst3_iid));
		cardinality = _cardinality;
		if (_category) strncpy8 (category, _category, PClassInfo::kCategorySize);
		if (_name) strncpy16 (name, _name, PClassInfo::kNameSize);
		classFlags = static_cast<uint32> (_classFlags);
		if (_subCategories) strncpy8 (subCategories, _subCategories, kSubCategoriesSize);
		if (_vendor) strncpy16 (vendor, _vendor, kVendorSize);
		if (_version) strncpy16 (version, _version, kVersionSize);
		if (_sdkVersion) strncpy16 (sdkVersion, _sdkVersion, kVersionSize);
	}
//#if SMTG_CPP11
	constexpr PClassInfoW ()
	: cid ()
	, cardinality ()
	, category ()
	, name ()
	, classFlags ()
	, subCategories ()
	, vendor ()
	, version ()
	, sdkVersion ()
	{
	}
//#else
//	PClassInfoW () { memset (this, 0, sizeof (PClassInfoW)); }
//#endif
	void fromAscii (const PClassInfo2& ci2) {
		//memcpy (cid, ci2.cid, sizeof (vst3_iid));
		//cardinality = ci2.cardinality;
		//strncpy8 (category, ci2.category, PClassInfo::kCategorySize);
		//str8ToStr16 (name, ci2.name, PClassInfo::kNameSize);
		//classFlags = ci2.classFlags;
		//strncpy8 (subCategories, ci2.subCategories, kSubCategoriesSize);
		//str8ToStr16(vendor, ci2.vendor, kVendorSize);
		//str8ToStr16(version, ci2.version, kVersionSize);
		//str8ToStr16(sdkVersion, ci2.sdkVersion, kVersionSize);
	}
};
*/

struct VST3_PClassInfoW {
};

struct VST3_PFactoryInfo {
  enum VST3_FactoryFlags {
    kNoFlags					        = 0,
    kClassesDiscardable			  = 1 << 0,
    kLicenseCheck				      = 1 << 1,
    kComponentNonDiscardable	= 1 << 3,
    kUnicode                  = 1 << 4
  };
  enum {
    kURLSize    = 256,
    kEmailSize  = 128,
    kNameSize   = 64
  };
  char    vendor[kNameSize];
  char    url[kURLSize];
  char    email[kEmailSize];
  int32_t flags;
  VST3_PFactoryInfo (const char* _vendor, const char* _url, const char* _email, int32_t _flags) {
    strncpy(vendor, _vendor, kNameSize);
    strncpy(url, _url, kURLSize);
    strncpy(email, _email, kEmailSize);
    flags = _flags;
//    #ifdef UNICODE
//    flags |= kUnicode;
//    #endif
  }
  //#if SMTG_CPP11
  constexpr VST3_PFactoryInfo() : vendor(), url(), email(), flags() {}
  //#else
  //  PFactoryInfo () { memset(this,0,sizeof(PFactoryInfo)); }
  //#endif
};

struct VST3_ProgramListInfo {
  int32_t       id;
  vst3_utf16str name;
  int32_t       programCount;
};

struct VST3_ProcessContext {
	enum VST3_StatesAndFlags {
		kPlaying                = 1 << 1,
		kCycleActive            = 1 << 2,
		kRecording              = 1 << 3,
		kSystemTimeValid        = 1 << 8,
		kContTimeValid          = 1 << 17,
		kProjectTimeMusicValid  = 1 << 9,
		kBarPositionValid       = 1 << 11,
		kCycleValid             = 1 << 12,
		kTempoValid             = 1 << 10,
		kTimeSigValid           = 1 << 13,
		kChordValid             = 1 << 18,
		kSmpteValid             = 1 << 14,
		kClockValid             = 1 << 15
	};
	uint32_t        state;
	double          sampleRate;
	int64_t         projectTimeSamples;
	int64_t         systemTime;
	int64_t         continousTimeSamples;
	double          projectTimeMusic;
	double          barPositionMusic;
	double          cycleStartMusic;
	double          cycleEndMusic;
	double          tempo;
	int32_t         timeSigNumerator;
	int32_t         timeSigDenominator;
	VST3_Chord      chord;
	int32_t         smpteOffsetSubframes;
	VST3_FrameRate frameRate;
	int32_t       samplesToNextClock;
};

struct VST3_ProcessData {
  VST3_ProcessData ()
    : processMode (0), symbolicSampleSize (kSample32), numSamples (0), numInputs (0)
    , numOutputs (0), inputs (0), outputs (0), inputParameterChanges (0), outputParameterChanges (0)
    , inputEvents (0), outputEvents (0), processContext (0) {}
  int32_t             processMode;
  int32_t             symbolicSampleSize;
  int32_t             numSamples;
  int32_t             numInputs;
  int32_t             numOutputs;
  VST3_AudioBusBuffers*    inputs;
  VST3_AudioBusBuffers*    outputs;
  IParameterChanges*  inputParameterChanges;
  IParameterChanges*  outputParameterChanges;
  IEventList*         inputEvents;
  IEventList*         outputEvents;
  VST3_ProcessContext*     processContext;
};

struct VST3_ProcessSetup {
  int32_t processMode;
  int32_t symbolicSampleSize;
  int32_t maxSamplesPerBlock;
  double  sampleRate;
};

struct VST3_RoutingInfo {
	int32_t mediaType;
	int32_t busIndex;
	int32_t channel;
};

struct VST3_UnitInfo {
	int32_t   id;
	int32_t   parentUnitId;
	vst3_utf16str name;
	int32_t   programListId;
};

struct VST3_ViewRect {
  VST3_ViewRect(int32_t l=0, int32_t t=0, int32_t r=0, int32_t b=0)
  : left(l), top(t), right(r), bottom(b) {}
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
  int32_t getWidth() const { return right - left; }
  int32_t getHeight() const { return bottom - top; }
};

//----------------------------------------------------------------------

struct NoteOnEvent {
  int16_t channel;
  int16_t pitch;
  float   tuning;
  float   velocity;
  int32_t length;
  int32_t noteId;
};

struct NoteOffEvent {
  int16_t channel;
  int16_t pitch;
  float   velocity;
  int32_t noteId;
  float   tuning;
};

struct DataEvent {
  uint32_t        size;
  uint32_t        type;
  const uint8_t*  bytes;
  enum DataTypes {
    kMidiSysEx = 0
  };
};

struct PolyPressureEvent {
  int16_t channel;
  int16_t pitch;
  float   pressure;
  int32_t noteId;
};

struct ChordEvent {
  int16_t         root;
  int16_t         bassNote;
  int16_t         mask;
  uint16_t        textLen;
  const char16_t* text;
};

struct ScaleEvent {
  int16_t         root;
  int16_t         mask;
  uint16_t        textLen;
  const char16_t* text;
};

struct NoteExpressionValueEvent {
  uint32_t  typeId;
  int32_t   noteId;
  double    value;
};

struct NoteExpressionTextEvent {
  uint32_t        typeId;
  int32_t         noteId;
  uint32_t        textLen;
  const char16_t* text;
};

struct Event {
  int32_t   busIndex;
  int32_t   sampleOffset;
  double    ppqPosition;
  uint16_t  flags;
  enum EventFlags {
    kIsLive         = 1 << 0,
    kUserReserved1  = 1 << 14,
    kUserReserved2  = 1 << 15
  };
  enum EventTypes
  {
    kNoteOnEvent = 0,
    kNoteOffEvent,
    kDataEvent,
    kPolyPressureEvent,
    kNoteExpressionValueEvent,
    kNoteExpressionTextEvent,
    kChordEvent,
    kScaleEvent
  };
  uint16_t type;
  union {
    NoteOnEvent               noteOn;
    NoteOffEvent              noteOff;
    DataEvent                 data;
    PolyPressureEvent         polyPressure;
    NoteExpressionValueEvent  noteExpressionValue;
    NoteExpressionTextEvent   noteExpressionText;
    ChordEvent                chord;
    ScaleEvent                scale;
  };
};

//----------------------------------------------------------------------
//
// utils
//
//----------------------------------------------------------------------

//#define char_to_utf16(C,U) UString128(C).copyTo(U,128)
//#define utf16_to_char(U,C) UString128(U).toAscii(C,128)

/*
#include <string>
#include <locale>
#include <codecvt>
//UTF-8 to UTF-16
std::string source;
std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
std::u16string dest = convert.from_bytes(source);
//UTF-16 to UTF-8
std::u16string source;
std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
std::string dest = convert.to_bytes(source);
*/

char16_t* vst3_str16cpy(char16_t* destination, const char16_t* source) {
  char16_t* temp = destination;
  while((*temp++ = *source++) != 0) ;
  return destination;
}

//----------

void vst3_char_to_utf16(const void* C, void* U) {
  std::string source = (char*)C;
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
  std::u16string dest = convert.from_bytes(source);
  vst3_str16cpy((char16_t*)U,dest.data());
}

//----------

void vst3_utf16_to_char(const void* U, void* C) {
  std::u16string source = (char16_t*)U;
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
  std::string dest = convert.to_bytes(source);
  strcpy((char*)C,dest.data());
}

//----------

bool vst3_iid_equal(const void* iid1, const void* iid2) {
  uint8_t* ptr1 = (uint8_t*)iid1;
  uint8_t* ptr2 = (uint8_t*)iid2;
  for (uint32_t i=0; i<16; i++) {
    if (*ptr1++ != *ptr2++) return false;
  }
  return true;
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class VST3_FUnknown {
public:
	virtual int32_t   VST3_PLUGIN_API queryInterface (const vst3_iid _iid, void** obj) = 0;
	virtual uint32_t  VST3_PLUGIN_API addRef () = 0;
	virtual uint32_t  VST3_PLUGIN_API release () = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID(FUnknown, 0x00000000, 0x00000000, 0xC0000000, 0x00000046)

//----------

class VST3_IBStream
: public VST3_FUnknown {
public:
  enum IStreamSeekMode {
    kIBSeekSet = 0,
    kIBSeekCur,
    kIBSeekEnd
  };
  virtual int32_t VST3_PLUGIN_API read(void* buffer, int32_t numBytes, int32_t* numBytesRead = 0) = 0;
  virtual int32_t VST3_PLUGIN_API write(void* buffer, int32_t numBytes, int32_t* numBytesWritten = 0) = 0;
  virtual int32_t VST3_PLUGIN_API seek(int64_t pos, int32_t mode, int64_t* result = 0) = 0;
  virtual int32_t VST3_PLUGIN_API tell(int64_t* pos) = 0;
  static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID(IBStream,0xC3BF6EA2,0x30994752,0x9B6BF990,0x1EE33E9B)

//----------

class VST3_IPluginFactory
: public VST3_FUnknown {
public:
  virtual int32_t  VST3_PLUGIN_API getFactoryInfo(PFactoryInfo* info) = 0;
  virtual int32_t  VST3_PLUGIN_API countClasses() = 0;
  virtual int32_t  VST3_PLUGIN_API getClassInfo (int32_t index, VST3_PClassInfo* info) = 0;
  virtual int32_t  VST3_PLUGIN_API createInstance(const char* cid, const char* _iid, void** obj) = 0;
  static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID(IPluginFactory, 0x7A4D811C, 0x52114A1F, 0xAED9D2EE, 0x0B43BF9F)

//----------

class VST3_IPluginFactory2
: public VST3_IPluginFactory {
public:
	virtual int32_t VST3_PLUGIN_API getClassInfo2 (int32_t index, VST3_PClassInfo2* info) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IPluginFactory2, 0x0007B650, 0xF24B4C0B, 0xA464EDB9, 0xF00B2ABB)

//----------

class VST3_IPluginFactory3
: public VST3_IPluginFactory2 {
public:
	virtual int32_t VST3_PLUGIN_API getClassInfoUnicode (int32_t index, VST3_PClassInfoW* info) = 0;
	virtual int32_t VST3_PLUGIN_API setHostContext (FUnknown* context) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IPluginFactory3, 0x4555A2AB, 0xC1234E57, 0x9B122910, 0x36878931)

//----------

class VST3_IPluginBase
: public VST3_FUnknown {
public:
  virtual int32_t  VST3_PLUGIN_API initialize(FUnknown* context) = 0;
  virtual int32_t  VST3_PLUGIN_API terminate() = 0;
  static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID(IPluginBase,0x22888DDB,0x156E45AE,0x8358B348,0x08190625)

//----------

class VST3_IComponent
: public VST3_IPluginBase {
public:
  virtual int32_t  VST3_PLUGIN_API getControllerClassId(vst3_iid classId) = 0;
  virtual int32_t  VST3_PLUGIN_API setIoMode(int32_t mode) = 0;
  virtual int32_t  VST3_PLUGIN_API getBusCount(int32_t type, int32_t dir) = 0;
  virtual int32_t  VST3_PLUGIN_API getBusInfo(int32_t type, int32_t dir, int32_t index, VST3_BusInfo& bus) = 0;
  virtual int32_t  VST3_PLUGIN_API getRoutingInfo(VST3_RoutingInfo& inInfo, VST3_RoutingInfo& outInfo) = 0;
  virtual int32_t  VST3_PLUGIN_API activateBus(int32_t type, int32_t dir, int32_t index, uint8_t state) = 0;
  virtual int32_t  VST3_PLUGIN_API setActive(uint8_t state) = 0;
  virtual int32_t  VST3_PLUGIN_API setState(IBStream* state) = 0;
  virtual int32_t  VST3_PLUGIN_API getState(IBStream* state) = 0;
  static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID(IComponent,0xE831FF31,0xF2D54301,0x928EBBEE,0x25697802)

//----------

class VST3_IAudioProcessor
: public VST3_FUnknown {
public:
  virtual int32_t  VST3_PLUGIN_API setBusArrangements(uint64_t* inputs, int32_t numIns, uint64_t* outputs, int32_t numOuts) = 0;
  virtual int32_t  VST3_PLUGIN_API getBusArrangement(int32_t dir, int32_t index, uint64_t& arr) = 0;
  virtual int32_t  VST3_PLUGIN_API canProcessSampleSize(int32_t symbolicSampleSize) = 0;
  virtual uint32_t VST3_PLUGIN_API getLatencySamples() = 0;
  virtual int32_t  VST3_PLUGIN_API setupProcessing(VST3_ProcessSetup& setup) = 0;
  virtual int32_t  VST3_PLUGIN_API setProcessing(uint8_t state) = 0;
  virtual int32_t  VST3_PLUGIN_API process(VST3_ProcessData& data) = 0;
  virtual uint32_t VST3_PLUGIN_API getTailSamples() = 0;
  static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID(IAudioProcessor,0x42043F99,0xB7DA453C,0xA569E79D,0x9AAEC33D)

//----------

class VST3_IUnitInfo
: public VST3_FUnknown {
public:
	virtual int32_t   VST3_PLUGIN_API getUnitCount() = 0;
	virtual int32_t VST3_PLUGIN_API getUnitInfo(int32_t unitIndex, VST3_UnitInfo& info) = 0;
	virtual int32_t   VST3_PLUGIN_API getProgramListCount() = 0;
	virtual int32_t VST3_PLUGIN_API getProgramListInfo(int32_t listIndex, ProgramListInfo& info) = 0;
	virtual int32_t VST3_PLUGIN_API getProgramName(int32_t listId, int32_t programIndex, vst3_utf16str name) = 0;
	virtual int32_t VST3_PLUGIN_API getProgramInfo(int32_t listId, int32_t programIndex, const char* attributeId, vst3_utf16str attributeValue) = 0;
	virtual int32_t VST3_PLUGIN_API hasProgramPitchNames(int32_t listId, int32_t programIndex) = 0;
	virtual int32_t VST3_PLUGIN_API getProgramPitchName(int32_t listId, int32_t programIndex, int16_t midiPitch, vst3_utf16str name) = 0;
	virtual int32_t  VST3_PLUGIN_API getSelectedUnit() = 0;
	virtual int32_t VST3_PLUGIN_API selectUnit(int32_t unitId) = 0;
	virtual int32_t VST3_PLUGIN_API getUnitByBus(int32_t type, int32_t dir, int32_t busIndex, int32_t channel, int32_t& unitId) = 0;
	virtual int32_t VST3_PLUGIN_API setUnitProgramData(int32_t listOrUnitId, int32_t programIndex, IBStream* data) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IUnitInfo, 0x3D4BD6B5, 0x913A4FD2, 0xA886E768, 0xA5EB92C1)

//----------

class VST3_IAttributeList
: public VST3_FUnknown {
public:
	typedef const char* AttrID;
	virtual int32_t VST3_PLUGIN_API setInt(AttrID id, int64_t value) = 0;
	virtual int32_t VST3_PLUGIN_API getInt(AttrID id, int64_t& value) = 0;
	virtual int32_t VST3_PLUGIN_API setFloat(AttrID id, double value) = 0;
	virtual int32_t VST3_PLUGIN_API getFloat(AttrID id, double& value) = 0;
	virtual int32_t VST3_PLUGIN_API setString(AttrID id, const char16_t* string) = 0;
	virtual int32_t VST3_PLUGIN_API getString(AttrID id, char16_t* string, uint32_t size) = 0;
	virtual int32_t VST3_PLUGIN_API setBinary(AttrID id, const void* data, uint32_t size) = 0;
	virtual int32_t VST3_PLUGIN_API getBinary(AttrID id, const void*& data, uint32_t& size) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IAttributeList, 0x1E5F0AEB, 0xCC7F4533, 0xA2544011, 0x38AD5EE4)

//----------

class VST3_IMessage
: public VST3_FUnknown {
public:
	virtual const char*       VST3_PLUGIN_API getMessageID () = 0;
	virtual void            VST3_PLUGIN_API setMessageID (const char* id) = 0;
	virtual IAttributeList* VST3_PLUGIN_API getAttributes () = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IMessage, 0x936F033B, 0xC6C047DB, 0xBB0882F8, 0x13C1E613)

//----------

class VST3_IConnectionPoint
: public VST3_FUnknown {
public:
	virtual int32_t VST3_PLUGIN_API connect (IConnectionPoint* other) = 0;
	virtual int32_t VST3_PLUGIN_API disconnect (IConnectionPoint* other) = 0;
	virtual int32_t VST3_PLUGIN_API notify (IMessage* message) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IConnectionPoint, 0x70A4156F, 0x6E6E4026, 0x989148BF, 0xAA60D8D1)

//----------

class VST3_IMidiMapping
: public VST3_FUnknown {
public:
	virtual int32_t VST3_PLUGIN_API getMidiControllerAssignment (int32_t busIndex, int16_t channel, int16_t midiControllerNumber, uint32_t& id) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IMidiMapping, 0xDF0FF9F7, 0x49B74669, 0xB63AB732, 0x7ADBF5E5)

//----------

class VST3_IKeyswitchController
: public VST3_FUnknown {
public:
	virtual int32_t   VST3_PLUGIN_API getKeyswitchCount (int32_t busIndex, int16_t channel) = 0;
	virtual int32_t VST3_PLUGIN_API getKeyswitchInfo (int32_t busIndex, int16_t channel, int32_t keySwitchIndex, VST3_KeyswitchInfo& info) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IKeyswitchController, 0x1F2F76D3, 0xBFFB4B96, 0xB99527A5, 0x5EBCCEF4)

//----------

//INoteExpressionController

//----------

class VST3_IComponentHandler
: public VST3_FUnknown {
public:
	virtual int32_t VST3_PLUGIN_API beginEdit (uint32_t id) = 0;
	virtual int32_t VST3_PLUGIN_API performEdit (uint32_t id, double valueNormalized) = 0;
	virtual int32_t VST3_PLUGIN_API endEdit (uint32_t id) = 0;
	virtual int32_t VST3_PLUGIN_API restartComponent (int32_t flags) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IComponentHandler, 0x93A0BEA3, 0x0BD045DB, 0x8E890B0C, 0xC1E46AC6)

//----------

class VST3_IComponentHandler2
: public VST3_FUnknown {
public:
	virtual int32_t VST3_PLUGIN_API setDirty (uint8_t state) = 0;
	virtual int32_t VST3_PLUGIN_API requestOpenEditor (const char* name = vst3_Editor) = 0;
	virtual int32_t VST3_PLUGIN_API startGroupEdit () = 0;
	virtual int32_t VST3_PLUGIN_API finishGroupEdit () = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IComponentHandler2, 0xF040B4B3, 0xA36045EC, 0xABCDC045, 0xB4D5A2CC)

//----------

class VST3_IPlugFrame;

class VST3_IPlugView
: public VST3_FUnknown {
public:
	virtual int32_t VST3_PLUGIN_API isPlatformTypeSupported (const char* type) = 0;
	virtual int32_t VST3_PLUGIN_API attached (void* parent, const char* type) = 0;
	virtual int32_t VST3_PLUGIN_API removed () = 0;
	virtual int32_t VST3_PLUGIN_API onWheel (float distance) = 0;
	virtual int32_t VST3_PLUGIN_API onKeyDown (char16_t key, int16_t keyCode, int16_t modifiers) = 0;
	virtual int32_t VST3_PLUGIN_API onKeyUp (char16_t key, int16_t keyCode, int16_t modifiers) = 0;
	virtual int32_t VST3_PLUGIN_API getSize (VST3_ViewRect* size) = 0;
	virtual int32_t VST3_PLUGIN_API onSize (VST3_ViewRect* newSize) = 0;
	virtual int32_t VST3_PLUGIN_API onFocus (uint8_t state) = 0;
	virtual int32_t VST3_PLUGIN_API setFrame (IPlugFrame* frame) = 0;
	virtual int32_t VST3_PLUGIN_API canResize () = 0;
	virtual int32_t VST3_PLUGIN_API checkSizeConstraint (VST3_ViewRect* rect) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IPlugView, 0x5BC32507, 0xD06049EA, 0xA6151B52, 0x2B755B29)

//----------

class VST3_IPlugFrame
: public VST3_FUnknown {
public:
	virtual int32_t VST3_PLUGIN_API resizeView(IPlugView* view, VST3_ViewRect* newSize) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IPlugFrame, 0x367FAF01, 0xAFA94693, 0x8D4DA2A0, 0xED0882A3)

//----------

#define setState setEditorState
#define getState getEditorState

class VST3_IEditController
: public VST3_IPluginBase {
public:
	virtual int32_t     VST3_PLUGIN_API setComponentState (IBStream* state) = 0;
	virtual int32_t     VST3_PLUGIN_API setState (IBStream* state) = 0;
	virtual int32_t     VST3_PLUGIN_API getState (IBStream* state) = 0;
	virtual int32_t     VST3_PLUGIN_API getParameterCount () = 0;
	virtual int32_t     VST3_PLUGIN_API getParameterInfo (int32_t paramIndex, VST3_ParameterInfo& info) = 0;
	virtual int32_t     VST3_PLUGIN_API getParamStringByValue (uint32_t id, double valueNormalized, vst3_utf16str string) = 0;
	virtual int32_t     VST3_PLUGIN_API getdoubleByString (uint32_t id, char16_t* string, double& valueNormalized) = 0;
	virtual double      VST3_PLUGIN_API normalizedParamToPlain (uint32_t id, double valueNormalized) = 0;
	virtual double      VST3_PLUGIN_API plainParamToNormalized (uint32_t id, double plainValue) = 0;
	virtual double      VST3_PLUGIN_API getParamNormalized (uint32_t id) = 0;
	virtual int32_t     VST3_PLUGIN_API setParamNormalized (uint32_t id, double value) = 0;
	virtual int32_t     VST3_PLUGIN_API setComponentHandler (IComponentHandler* handler) = 0;
	virtual VST3_IPlugView*  VST3_PLUGIN_API createView (const char* name) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IEditController, 0xDCD7BBE3, 0x7742448D, 0xA874AACC, 0x979C759E)

#undef setState
#undef getState

//----------

class VST3_IEditController2
: public VST3_FUnknown {
public:
	virtual int32_t VST3_PLUGIN_API setKnobMode (int32_t mode) = 0;
	virtual int32_t VST3_PLUGIN_API openHelp (uint8_t onlyCheck) = 0;
	virtual int32_t VST3_PLUGIN_API openAboutBox (uint8_t onlyCheck) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IEditController2, 0x7F4EFE59, 0xF3204967, 0xAC27A3AE, 0xAFB63038)

//----------

class VST3_ITimerHandler
: public VST3_FUnknown {
public:
	virtual void VST3_PLUGIN_API onTimer () = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (ITimerHandler, 0x10BDD94F, 0x41424774, 0x821FAD8F, 0xECA72CA9)

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class VST3_IHostApplication
: public VST3_FUnknown {
public:
	virtual int32_t VST3_PLUGIN_API getName (vst3_utf16str name) = 0;
	virtual int32_t VST3_PLUGIN_API createInstance (vst3_iid cid, vst3_iid _iid, void** obj) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IHostApplication, 0x58E595CC, 0xDB2D4969, 0x8B6AAF8C, 0x36A664E5)

//----------

class VST3_IEventHandler
: public VST3_FUnknown {
public:
	virtual void VST3_PLUGIN_API onFDIsSet (int fd) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IEventHandler, 0x561E65C9, 0x13A0496F, 0x813A2C35, 0x654D7983)

//----------

class VST3_IRunLoop
: public VST3_FUnknown {
public:
	virtual int32_t VST3_PLUGIN_API registerEventHandler (IEventHandler* handler, int fd) = 0;
	virtual int32_t VST3_PLUGIN_API unregisterEventHandler (IEventHandler* handler) = 0;
	virtual int32_t VST3_PLUGIN_API registerTimer (ITimerHandler* handler, uint64_t milliseconds) = 0;
	virtual int32_t VST3_PLUGIN_API unregisterTimer (ITimerHandler* handler) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (IRunLoop, 0x18C35366, 0x97764F1A, 0x9C5B8385, 0x7A871389)

//----------

class VST3_IParamValueQueue
: public VST3_FUnknown {
public:
	virtual uint32_t  VST3_PLUGIN_API getParameterId () = 0;
	virtual int32_t   VST3_PLUGIN_API getPointCount () = 0;
	virtual int32_t   VST3_PLUGIN_API getPoint (int32_t index, int32_t& sampleOffset, double& value) = 0;
	virtual int32_t   VST3_PLUGIN_API addPoint (int32_t sampleOffset, double value, int32_t& index) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (VST3_IParamValueQueue, 0x01263A18, 0xED074F6F, 0x98C9D356, 0x4686F9BA)

//----------

class VST3_IParameterChanges
: public VST3_FUnknown {
public:
	virtual int32_t           VST3_PLUGIN_API getParameterCount () = 0;
	virtual VST3_IParamValueQueue* VST3_PLUGIN_API getParameterData (int32_t index) = 0;
	virtual VST3_IParamValueQueue* VST3_PLUGIN_API addParameterData (const uint32_t& id, int32_t& index) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (VST3_IParameterChanges, 0xA4779663, 0x0BB64A56, 0xB44384A8, 0x466FEB9D)

//----------

class VST3_IEventList
: public VST3_FUnknown {
public:
	virtual int32_t VST3_PLUGIN_API getEventCount () = 0;
	virtual int32_t VST3_PLUGIN_API getEvent (int32_t index, Event& e) = 0;
	virtual int32_t VST3_PLUGIN_API addEvent (Event& e) = 0;
	static const vst3_iid iid;
};

VST3_DECLARE_CLASS_IID (VST3_IEventList, 0x3A2C4214, 0x346349FE, 0xB2C4F397, 0xB9695A44)

//----------------------------------------------------------------------
#endif

