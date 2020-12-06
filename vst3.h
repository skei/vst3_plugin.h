#ifndef vst3_included
#define vst3_included
//----------------------------------------------------------------------

#ifdef __gnu_linux__
  //#define VST3_LINUX
  #define COM_COMPATIBLE 0
  #define PLUGIN_API
#endif

#ifdef _WIN32
  //#define VST3_WIN32
  #define COM_COMPATIBLE 1
  #define PLUGIN_API __stdcal
#endif

//----------

#define strncpy8 strncpy

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#if COM_COMPATIBLE
  #define INLINE_UID(l1,l2,l3,l4) {                                  \
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
  #define INLINE_UID(l1, l2, l3, l4) {                               \
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

//#ifdef INIT_CLASS_IID
//  #define DECLARE_CLASS_IID(classname, l1, l2, l3, l4)
//	static const vst3_uuid classname##_iid = INLINE_UID (l1,l2,l3,l4);
//  const vst3_uuid classname::iid (classname##_iid);
//#else

#define DECLARE_CLASS_IID(classname,l1,l2,l3,l4) \
  static const TUID classname##_iid = INLINE_UID(l1,l2,l3,l4);

//----------

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

typedef char                int8;
typedef short               int16;
typedef int                 int32;
typedef long long           int64;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

//typedef long                int32;
//typedef __int64             int64;
//typedef unsigned long       uint32;
//typedef unsigned __int64    uint64;

typedef char                char8;
typedef char16_t            char16;
typedef unsigned char       uchar;
typedef char8               tchar;

//typedef int16               char16;
//typedef __wchar_t           char16;
//typedef char16              tchar;

typedef char16              TChar;
typedef TChar               String128[128];

typedef int32_t             tresult;

typedef const char*         FIDString;
typedef uint8_t             TBool;
typedef double              TQuarterNotes;
typedef int64_t             TSamples;

//typedef int8                TUID[16];
typedef char                TUID[16];
typedef TUID                FUID;

typedef int32_t             BusDirection;
typedef int32_t             BusType;
typedef const char8*        CString;
typedef int16_t             CtrlNumber;
typedef int                 FileDescriptor;
typedef int32_t             IoMode;
typedef uint32              KeyswitchTypeID;
typedef int32_t             KnobMode;
typedef int32_t             MediaType;
typedef double              NoteExpressionValue;
typedef uint32              NoteExpressionTypeID;
typedef uint32_t            ParamID;
typedef double              ParamValue;
typedef int32_t             ProgramListID;
typedef float               Sample32;
typedef double              Sample64;
typedef double              SampleRate;
typedef uint64              Speaker;
typedef uint64              SpeakerArrangement;
typedef uint64_t            TimerInterval;
typedef int32_t             UnitID;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef kVstVersionString
#define kVstVersionString	"VST 3.6.10"	///< SDK version for PClassInfo2
#endif

#ifndef kVstAudioEffectClass
  #define kVstAudioEffectClass "Audio Module Class"
#endif

//----------

#if COM_COMPATIBLE

  #ifdef _WIN32

    enum {
      kNoInterface		  = static_cast<tresult>(0x80004002L),	// E_NOINTERFACE
      kResultOk         = static_cast<tresult>(0x00000000L),	// S_OK
      kResultTrue       = kResultOk,
      kResultFalse		  = static_cast<tresult>(0x00000001L),	// S_FALSE
      kInvalidArgument	= static_cast<tresult>(0x80070057L),	// E_INVALIDARG
      kNotImplemented		= static_cast<tresult>(0x80004001L),	// E_NOTIMPL
      kInternalError		= static_cast<tresult>(0x80004005L),	// E_FAIL
      kNotInitialized		= static_cast<tresult>(0x8000FFFFL),	// E_UNEXPECTED
      kOutOfMemory		  = static_cast<tresult>(0x8007000EL)		// E_OUTOFMEMORY
    };

  #else // _WIN32

    enum {
      kNoInterface      = static_cast<tresult>(0x80000004L),	// E_NOINTERFACE
      kResultOk         = static_cast<tresult>(0x00000000L),	// S_OK
      kResultTrue       = kResultOk,
      kResultFalse      = static_cast<tresult>(0x00000001L),	// S_FALSE
      kInvalidArgument  = static_cast<tresult>(0x80000003L),	// E_INVALIDARG
      kNotImplemented   = static_cast<tresult>(0x80000001L),	// E_NOTIMPL
      kInternalError    = static_cast<tresult>(0x80000008L),	// E_FAIL
      kNotInitialized   = static_cast<tresult>(0x8000FFFFL),	// E_UNEXPECTED
      kOutOfMemory      = static_cast<tresult>(0x80000002L)		// E_OUTOFMEMORY
    };

  #endif // _WIN32

#else // COM_COMPATIBLE

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

#endif // COM_COMPATIBLE

//----------------------------------------------------------------------

        const char*         kEditor           = "editor";
        const CString       kFx					      = "Fx";
        const CString       kInstrument			  = "Instrument";
static  const UnitID        kNoParentUnitId   = -1;
static  const ProgramListID kNoProgramListId  = -1;
static  const uint32        kNoTail           = 0;
static  const UnitID        kRootUnitId       = 0;
        const Speaker       kSpeakerL         = 1 << 0;
        const Speaker       kSpeakerR         = 1 << 1;

//----------------------------------------------------------------------

//namespace PlugType {
//  const CString kFxAnalyzer			      = "Fx|Analyzer";
//  const CString kFxDelay				      = "Fx|Delay";
//  const CString kFxDistortion		      = "Fx|Distortion";
//  const CString kFxDynamics			      = "Fx|Dynamics";
//  const CString kFxEQ					        = "Fx|EQ";
//  const CString kFxFilter				      = "Fx|Filter";
//  const CString kFx					          = "Fx";
//  const CString kFxInstrument			    = "Fx|Instrument";
//  const CString kFxInstrumentExternal	= "Fx|Instrument|External";
//  const CString kFxSpatial			      = "Fx|Spatial";
//  const CString kFxGenerator			    = "Fx|Generator";
//  const CString kFxMastering			    = "Fx|Mastering";
//  const CString kFxModulation			= "Fx|Modulation";
//  const CString kFxPitchShift			= "Fx|Pitch Shift";
//  const CString kFxRestoration		= "Fx|Restoration";
//  const CString kFxReverb				= "Fx|Reverb";
//  const CString kFxSurround			= "Fx|Surround";
//  const CString kFxTools				= "Fx|Tools";
//  const CString kFxNetwork			= "Fx|Network";
//  const CString kInstrument			= "Instrument";
//  const CString kInstrumentDrum		= "Instrument|Drum";
//  const CString kInstrumentSampler	= "Instrument|Sampler";
//  const CString kInstrumentSynth		= "Instrument|Synth";
//  const CString kInstrumentSynthSampler = "Instrument|Synth|Sampler";
//  const CString kInstrumentExternal	= "Instrument|External";
//  const CString kSpatial				= "Spatial";
//  const CString kSpatialFx			= "Spatial|Fx";
//  const CString kOnlyRealTime			= "OnlyRT";
//  const CString kOnlyOfflineProcess	= "OnlyOfflineProcess";
//  const CString kNoOfflineProcess		= "NoOfflineProcess";
//  const CString kUpDownMix			= "Up-Downmix";
//  const CString kAnalyzer			    = "Analyzer";
//  const CString kAmbisonic			= "Ambisonic";
//  const CString kMono					= "Mono";
//  const CString kStereo				= "Stereo";
//  const CString kSurround				= "Surround";
//};

//----------------------------------------------------------------------

//enum NoteExpressionTypeIDs {
//  kVolumeTypeID = 0,
//  kPanTypeID,
//  kTuningTypeID,
//  kVibratoTypeID,
//  kExpressionTypeID,
//  kBrightnessTypeID,
//  kTextTypeID,
//  kPhonemeTypeID,
//  kCustomStart = 100000
//};

//----------

enum BusDirections {
  kInput = 0,
  kOutput
};

enum BusTypes {
  kMain = 0,
  kAux
};

enum ControllerNumbers {
  kCtrlBankSelectMSB	=	0,
  kCtrlModWheel		=	1,
  kCtrlBreath			=	2,
  kCtrlFoot			=	4,
  kCtrlPortaTime		=	5,
  kCtrlDataEntryMSB	=	6,
  kCtrlVolume			=	7,
  kCtrlBalance		=	8,
  kCtrlPan			=	10,
  kCtrlExpression		=	11,
  kCtrlEffect1		=	12,
  kCtrlEffect2		=	13,
  kCtrlGPC1			=	16,
  kCtrlGPC2			=	17,
  kCtrlGPC3			=	18,
  kCtrlGPC4			=	19,
  kCtrlBankSelectLSB	=	32,
  kCtrlDataEntryLSB	=	38,
  kCtrlSustainOnOff	=	64,
  kCtrlPortaOnOff		=	65,
  kCtrlSustenutoOnOff	=	66,
  kCtrlSoftPedalOnOff	=	67,
  kCtrlLegatoFootSwOnOff=	68,
  kCtrlHold2OnOff		=	69,
  kCtrlSoundVariation	=	70,
  kCtrlFilterCutoff	=	71,
  kCtrlReleaseTime	=	72,
  kCtrlAttackTime		=	73,
  kCtrlFilterResonance=	74,
  kCtrlDecayTime		=	75,
  kCtrlVibratoRate	=	76,
  kCtrlVibratoDepth	=	77,
  kCtrlVibratoDelay	=	78,
  kCtrlSoundCtrler10	=	79,
  kCtrlGPC5			=	80,
  kCtrlGPC6			=	81,
  kCtrlGPC7			=	82,
  kCtrlGPC8			=	83,
  kCtrlPortaControl	=	84,
  kCtrlEff1Depth		=	91,
  kCtrlEff2Depth		=	92,
  kCtrlEff3Depth		=	93,
  kCtrlEff4Depth		=	94,
  kCtrlEff5Depth		=	95,
  kCtrlDataIncrement	=	96,
  kCtrlDataDecrement	=	97,
  kCtrlNRPNSelectLSB 	=	98,
  kCtrlNRPNSelectMSB	=	99,
  kCtrlRPNSelectLSB	=	100,
  kCtrlRPNSelectMSB	=	101,
  kCtrlAllSoundsOff	=	120,
  kCtrlResetAllCtrlers =	121,
  kCtrlLocalCtrlOnOff	=	122,
  kCtrlAllNotesOff	=	123,
  kCtrlOmniModeOff	=	124,
  kCtrlOmniModeOn		=	125,
  kCtrlPolyModeOnOff	=	126,
  kCtrlPolyModeOn		=	127,
  kAfterTouch = 128,
  kPitchBend,
  kCountCtrlNumber
};

enum MediaTypes {
  kAudio = 0,
  kEvent,
  kNumMediaTypes
};

enum SymbolicSampleSizes {
  kSample32,
  kSample64
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class IParameterChanges;
class IEventList;

//----------

struct AudioBusBuffers {
  AudioBusBuffers () : numChannels (0), silenceFlags (0), channelBuffers64 (0) {}
  int32 numChannels;
  uint64 silenceFlags;
  union {
    Sample32** channelBuffers32;
    Sample64** channelBuffers64;
  };
};

struct BusInfo {
  MediaType mediaType;
  BusDirection direction;
  int32 channelCount;
  String128 name;
  BusType busType;
  uint32 flags;
  enum BusFlags 	{
    kDefaultActive = 1 << 0
  };
};

struct Chord {
  uint8 keyNote;
  uint8 rootNote;
  int16 chordMask;
  enum Masks {
    kChordMask    = 0x0FFF,
    kReservedMask = 0xF000
  };
};

struct FrameRate {
  enum FrameRateFlags {
    kPullDownRate = 1 << 0,
    kDropRate     = 1 << 1
  };
  uint32  framesPerSecond;
  uint32  flags;
};

struct KeyswitchInfo {
  KeyswitchTypeID typeId;
  String128 title;
  String128 shortTitle;
  int32 keyswitchMin;
  int32 keyswitchMax;
  int32 keyRemapped;
  int32 unitId;
  int32 flags;
};

struct ParameterInfo {
  ParamID     id;
  String128   title;
  String128   shortTitle;
  String128   units;
  int32       stepCount;
  ParamValue  defaultNormalizedValue;
  UnitID      unitId;
  int32       flags;
  enum ParameterFlags {
    kCanAutomate      = 1 << 0,
    kIsReadOnly       = 1 << 1,
    kIsWrapAround     = 1 << 2,
    kIsList           = 1 << 3,
    kIsProgramChange  = 1 << 15,
    kIsBypass         = 1 << 16
  };
};

struct PClassInfo {
  enum ClassCardinality {
    kManyInstances = 0x7FFFFFFF
  };
  enum {
    kCategorySize = 32,
    kNameSize = 64
  };
  TUID cid;
  int32 cardinality;
  char8 category[kCategorySize];
  char8 name[kNameSize];
  PClassInfo(const TUID _cid, int32 _cardinality, const char8* _category, const char8* _name) {
    memset (this, 0, sizeof (PClassInfo));
    memcpy (cid, _cid, sizeof (TUID));
    if (_category) strncpy8 (category, _category, kCategorySize);
    if (_name) strncpy8 (name, _name, kNameSize);
    cardinality = _cardinality;
  }
  //#if SMTG_CPP11
  constexpr PClassInfo() : cid(), cardinality(), category(), name() {}
  //#else
  //PClassInfo () { memset (this, 0, sizeof (PClassInfo)); }
  //#endif
};

struct PClassInfo2 {
	TUID cid;
	int32 cardinality;
	char8 category[PClassInfo::kCategorySize];
	char8 name[PClassInfo::kNameSize];
	enum {
		kVendorSize = 64,
		kVersionSize = 64,
		kSubCategoriesSize = 128
	};
	uint32 classFlags;
	char8 subCategories[kSubCategoriesSize];
	char8 vendor[kVendorSize];
	char8 version[kVersionSize];
	char8 sdkVersion[kVersionSize];
	PClassInfo2 (const TUID _cid, int32 _cardinality, const char8* _category, const char8* _name,
		int32 _classFlags, const char8* _subCategories, const char8* _vendor, const char8* _version,
		const char8* _sdkVersion) {
		memset (this, 0, sizeof (PClassInfo2));
		memcpy (cid, _cid, sizeof (TUID));
		cardinality = _cardinality;
		if (_category) strncpy8 (category, _category, PClassInfo::kCategorySize);
		if (_name) strncpy8 (name, _name, PClassInfo::kNameSize);
		classFlags = static_cast<uint32> (_classFlags);
		if (_subCategories) strncpy8 (subCategories, _subCategories, kSubCategoriesSize);
		if (_vendor) strncpy8 (vendor, _vendor, kVendorSize);
		if (_version) strncpy8 (version, _version, kVersionSize);
		if (_sdkVersion) strncpy8 (sdkVersion, _sdkVersion, kVersionSize);
	}
//#if SMTG_CPP11
	constexpr PClassInfo2 ()
	: cid(), cardinality(), category(), name(), classFlags(), subCategories(), vendor(), version(), sdkVersion() {}
//#else
//	PClassInfo2 () { memset (this, 0, sizeof (PClassInfo2)); }
//#endif
};

/*
struct PClassInfoW {
	TUID cid;
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
	PClassInfoW (const TUID _cid, int32 _cardinality, const char8* _category, const char16* _name,
		int32 _classFlags, const char8* _subCategories, const char16* _vendor, const char16* _version,
		const char16* _sdkVersion) {
		memset (this, 0, sizeof (PClassInfoW));
		memcpy (cid, _cid, sizeof (TUID));
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
		//memcpy (cid, ci2.cid, sizeof (TUID));
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

struct PClassInfoW {
};

struct PFactoryInfo {
  enum FactoryFlags {
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
  char8 vendor[kNameSize];
  char8 url[kURLSize];
  char8 email[kEmailSize];
  int32 flags;
  PFactoryInfo (const char8* _vendor, const char8* _url, const char8* _email, int32 _flags) {
    strncpy8(vendor, _vendor, kNameSize);
    strncpy8(url, _url, kURLSize);
    strncpy8(email, _email, kEmailSize);
    flags = _flags;
//    #ifdef UNICODE
//    flags |= kUnicode;
//    #endif
  }
  //#if SMTG_CPP11
  constexpr PFactoryInfo() : vendor(), url(), email(), flags() {}
  //#else
  //  PFactoryInfo () { memset(this,0,sizeof(PFactoryInfo)); }
  //#endif
};

struct ProgramListInfo {
  ProgramListID id;
  String128 name;
  int32 programCount;
};

struct ProcessContext {
	enum StatesAndFlags {
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
	uint32        state;
	double        sampleRate;
	TSamples      projectTimeSamples;
	int64         systemTime;
	TSamples      continousTimeSamples;
	TQuarterNotes projectTimeMusic;
	TQuarterNotes barPositionMusic;
	TQuarterNotes cycleStartMusic;
	TQuarterNotes cycleEndMusic;
	double        tempo;
	int32         timeSigNumerator;
	int32         timeSigDenominator;
	Chord         chord;
	int32         smpteOffsetSubframes;
	FrameRate     frameRate;
	int32         samplesToNextClock;
};

struct ProcessData {
  ProcessData ()
    : processMode (0), symbolicSampleSize (kSample32), numSamples (0), numInputs (0)
    , numOutputs (0), inputs (0), outputs (0), inputParameterChanges (0), outputParameterChanges (0)
    , inputEvents (0), outputEvents (0), processContext (0) {}
  int32 processMode;
  int32 symbolicSampleSize;
  int32 numSamples;
  int32 numInputs;
  int32 numOutputs;
  AudioBusBuffers* inputs;
  AudioBusBuffers* outputs;
  IParameterChanges* inputParameterChanges;
  IParameterChanges* outputParameterChanges;
  IEventList* inputEvents;
  IEventList* outputEvents;
  ProcessContext* processContext;
};

struct ProcessSetup {
  int32 processMode;
  int32 symbolicSampleSize;
  int32 maxSamplesPerBlock;
  SampleRate sampleRate;
};

struct RoutingInfo {
	MediaType mediaType;
	int32 busIndex;
	int32 channel;
};

struct UnitInfo {
	UnitID id;
	UnitID parentUnitId;
	String128 name;
	ProgramListID programListId;
};

struct ViewRect {
  //ViewRect(int32 l=0, int32 t=0, int32 r=0, int32 b=0)
  //: left(l), top(t), right(r), bottom(b) {}
  int32 left;
  int32 top;
  int32 right;
  int32 bottom;
  //int32 getWidth() const { return right - left; }
  //int32 getHeight() const { return bottom - top; }
};

//----------------------------------------------------------------------

struct NoteOnEvent {
  int16 channel;
  int16 pitch;
  float tuning;
  float velocity;
  int32 length;
  int32 noteId;
};

struct NoteOffEvent {
  int16 channel;
  int16 pitch;
  float velocity;
  int32 noteId;
  float tuning;
};

struct DataEvent {
  uint32 size;
  uint32 type;
  const uint8* bytes;
  enum DataTypes {
    kMidiSysEx = 0
  };
};

struct PolyPressureEvent {
  int16 channel;
  int16 pitch;
  float pressure;
  int32 noteId;
};

struct ChordEvent {
  int16 root;
  int16 bassNote;
  int16 mask;
  uint16 textLen;
  const TChar* text;
};

struct ScaleEvent {
  int16 root;
  int16 mask;
  uint16 textLen;
  const TChar* text;
};

struct NoteExpressionValueEvent {
  NoteExpressionTypeID typeId;
  int32 noteId;
  NoteExpressionValue value;
};

struct NoteExpressionTextEvent {
  NoteExpressionTypeID typeId;
  int32 noteId;
  uint32 textLen;
  const TChar* text;
};

struct Event {
  int32 busIndex;
  int32 sampleOffset;
  TQuarterNotes ppqPosition;
  uint16 flags;
  enum EventFlags {
    kIsLive = 1 << 0,
    kUserReserved1 = 1 << 14,
    kUserReserved2 = 1 << 15
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
  uint16 type;
  union {
    NoteOnEvent noteOn;
    NoteOffEvent noteOff;
    DataEvent data;
    PolyPressureEvent polyPressure;
    NoteExpressionValueEvent noteExpressionValue;
    NoteExpressionTextEvent noteExpressionText;
    ChordEvent chord;
    ScaleEvent scale;
  };
};

//----------------------------------------------------------------------
//
// utils
//
//----------------------------------------------------------------------

//#define char_to_utf16(C,U) UString128(C).copyTo(U,128)
//#define utf16_to_char(U,C) UString128(U).toAscii(C,128)

void  char_to_utf16(const void* C, void* U) {
}

//----------

void utf16_to_char(const void* U, void* C) {
}

//

bool iidEqual(const void* iid1, const void* iid2) {
  return false;
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class FUnknown {
public:
	virtual tresult   PLUGIN_API queryInterface (const TUID _iid, void** obj) = 0;
	virtual uint32_t  PLUGIN_API addRef () = 0;
	virtual uint32_t  PLUGIN_API release () = 0;
	static const TUID iid;
};

DECLARE_CLASS_IID(FUnknown, 0x00000000, 0x00000000, 0xC0000000, 0x00000046)

//----------

class IBStream
: public FUnknown {
public:
  enum IStreamSeekMode {
    kIBSeekSet = 0,
    kIBSeekCur,
    kIBSeekEnd
  };
  virtual tresult PLUGIN_API read(void* buffer, int32_t numBytes, int32_t* numBytesRead = 0) = 0;
  virtual tresult PLUGIN_API write(void* buffer, int32_t numBytes, int32_t* numBytesWritten = 0) = 0;
  virtual tresult PLUGIN_API seek(int64_t pos, int32_t mode, int64_t* result = 0) = 0;
  virtual tresult PLUGIN_API tell(int64_t* pos) = 0;
  static const TUID iid;
};

DECLARE_CLASS_IID(IBStream,0xC3BF6EA2,0x30994752,0x9B6BF990,0x1EE33E9B)

//----------

class IPluginFactory
: public FUnknown {
public:
  virtual tresult  PLUGIN_API getFactoryInfo(PFactoryInfo* info) = 0;
  virtual int32_t  PLUGIN_API countClasses() = 0;
  virtual tresult  PLUGIN_API getClassInfo (int32_t index, PClassInfo* info) = 0;
  virtual tresult  PLUGIN_API createInstance(FIDString cid, FIDString _iid, void** obj) = 0;
  static const TUID iid;
};

DECLARE_CLASS_IID(IPluginFactory, 0x7A4D811C, 0x52114A1F, 0xAED9D2EE, 0x0B43BF9F)

//----------

class IPluginFactory2
: public IPluginFactory {
public:
	virtual tresult PLUGIN_API getClassInfo2 (int32 index, PClassInfo2* info) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IPluginFactory2, 0x0007B650, 0xF24B4C0B, 0xA464EDB9, 0xF00B2ABB)

//----------

class IPluginFactory3
: public IPluginFactory2 {
public:
	virtual tresult PLUGIN_API getClassInfoUnicode (int32 index, PClassInfoW* info) = 0;
	virtual tresult PLUGIN_API setHostContext (FUnknown* context) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IPluginFactory3, 0x4555A2AB, 0xC1234E57, 0x9B122910, 0x36878931)

//----------

class IPluginBase
: public FUnknown {
public:
  virtual tresult  PLUGIN_API initialize(FUnknown* context) = 0;
  virtual tresult  PLUGIN_API terminate() = 0;
  static const TUID iid;
};

DECLARE_CLASS_IID(IPluginBase,0x22888DDB,0x156E45AE,0x8358B348,0x08190625)

//----------

class IComponent
: public IPluginBase {
public:
  virtual tresult  PLUGIN_API getControllerClassId(TUID classId) = 0;
  virtual tresult  PLUGIN_API setIoMode(IoMode mode) = 0;
  virtual int32_t  PLUGIN_API getBusCount(MediaType type, BusDirection dir) = 0;
  virtual tresult  PLUGIN_API getBusInfo(MediaType type, BusDirection dir, int32_t index, BusInfo& bus) = 0;
  virtual tresult  PLUGIN_API getRoutingInfo(RoutingInfo& inInfo, RoutingInfo& outInfo) = 0;
  virtual tresult  PLUGIN_API activateBus(MediaType type, BusDirection dir, int32_t index, TBool state) = 0;
  virtual tresult  PLUGIN_API setActive(TBool state) = 0;
  virtual tresult  PLUGIN_API setState(IBStream* state) = 0;
  virtual tresult  PLUGIN_API getState(IBStream* state) = 0;
  static const TUID iid;
};

DECLARE_CLASS_IID(IComponent,0xE831FF31,0xF2D54301,0x928EBBEE,0x25697802)

//----------

class IAudioProcessor
: public FUnknown {
public:
  virtual tresult  PLUGIN_API setBusArrangements(SpeakerArrangement* inputs, int32_t numIns, SpeakerArrangement* outputs, int32_t numOuts) = 0;
  virtual tresult  PLUGIN_API getBusArrangement(BusDirection dir, int32_t index, SpeakerArrangement& arr) = 0;
  virtual tresult  PLUGIN_API canProcessSampleSize(int32_t symbolicSampleSize) = 0;
  virtual uint32_t PLUGIN_API getLatencySamples() = 0;
  virtual tresult  PLUGIN_API setupProcessing(ProcessSetup& setup) = 0;
  virtual tresult  PLUGIN_API setProcessing(TBool state) = 0;
  virtual tresult  PLUGIN_API process(ProcessData& data) = 0;
  virtual uint32_t PLUGIN_API getTailSamples() = 0;
  static const TUID iid;
};

DECLARE_CLASS_IID(IAudioProcessor,0x42043F99,0xB7DA453C,0xA569E79D,0x9AAEC33D)

//----------

class IUnitInfo
: public FUnknown {
public:
	virtual int32   PLUGIN_API getUnitCount() = 0;
	virtual tresult PLUGIN_API getUnitInfo(int32 unitIndex, UnitInfo& info /*out*/) = 0;
	virtual int32   PLUGIN_API getProgramListCount() = 0;
	virtual tresult PLUGIN_API getProgramListInfo(int32 listIndex, ProgramListInfo& info /*out*/) = 0;
	virtual tresult PLUGIN_API getProgramName(ProgramListID listId, int32 programIndex, String128 name /*out*/) = 0;
	virtual tresult PLUGIN_API getProgramInfo(ProgramListID listId, int32 programIndex, CString attributeId /*in*/, String128 attributeValue /*out*/) = 0;
	virtual tresult PLUGIN_API hasProgramPitchNames(ProgramListID listId, int32 programIndex) = 0;
	virtual tresult PLUGIN_API getProgramPitchName(ProgramListID listId, int32 programIndex, int16 midiPitch, String128 name /*out*/) = 0;
	virtual UnitID  PLUGIN_API getSelectedUnit() = 0;
	virtual tresult PLUGIN_API selectUnit(UnitID unitId) = 0;
	virtual tresult PLUGIN_API getUnitByBus(MediaType type, BusDirection dir, int32 busIndex, int32 channel, UnitID& unitId /*out*/) = 0;
	virtual tresult PLUGIN_API setUnitProgramData(int32 listOrUnitId, int32 programIndex, IBStream* data) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IUnitInfo, 0x3D4BD6B5, 0x913A4FD2, 0xA886E768, 0xA5EB92C1)

//----------

class IAttributeList
: public FUnknown {
public:
	typedef const char* AttrID;
	virtual tresult PLUGIN_API setInt(AttrID id, int64 value) = 0;
	virtual tresult PLUGIN_API getInt(AttrID id, int64& value) = 0;
	virtual tresult PLUGIN_API setFloat(AttrID id, double value) = 0;
	virtual tresult PLUGIN_API getFloat(AttrID id, double& value) = 0;
	virtual tresult PLUGIN_API setString(AttrID id, const TChar* string) = 0;
	virtual tresult PLUGIN_API getString(AttrID id, TChar* string, uint32 size) = 0;
	virtual tresult PLUGIN_API setBinary(AttrID id, const void* data, uint32 size) = 0;
	virtual tresult PLUGIN_API getBinary(AttrID id, const void*& data, uint32& size) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IAttributeList, 0x1E5F0AEB, 0xCC7F4533, 0xA2544011, 0x38AD5EE4)

//----------

class IMessage
: public FUnknown {
public:
	virtual FIDString       PLUGIN_API getMessageID () = 0;
	virtual void            PLUGIN_API setMessageID (FIDString id /*in*/) = 0;
	virtual IAttributeList* PLUGIN_API getAttributes () = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IMessage, 0x936F033B, 0xC6C047DB, 0xBB0882F8, 0x13C1E613)

//----------

class IConnectionPoint
: public FUnknown {
public:
	virtual tresult PLUGIN_API connect (IConnectionPoint* other) = 0;
	virtual tresult PLUGIN_API disconnect (IConnectionPoint* other) = 0;
	virtual tresult PLUGIN_API notify (IMessage* message) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IConnectionPoint, 0x70A4156F, 0x6E6E4026, 0x989148BF, 0xAA60D8D1)

//----------

class IMidiMapping
: public FUnknown {
public:
	virtual tresult PLUGIN_API getMidiControllerAssignment (int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id/*out*/) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IMidiMapping, 0xDF0FF9F7, 0x49B74669, 0xB63AB732, 0x7ADBF5E5)

//----------

class IKeyswitchController
: public FUnknown {
public:
	virtual int32   PLUGIN_API getKeyswitchCount (int32 busIndex, int16 channel) = 0;
	virtual tresult PLUGIN_API getKeyswitchInfo (int32 busIndex, int16 channel, int32 keySwitchIndex, KeyswitchInfo& info /*out*/) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IKeyswitchController, 0x1F2F76D3, 0xBFFB4B96, 0xB99527A5, 0x5EBCCEF4)

//----------

//INoteExpressionController

//----------

class IComponentHandler
: public FUnknown {
public:
	virtual tresult PLUGIN_API beginEdit (ParamID id) = 0;
	virtual tresult PLUGIN_API performEdit (ParamID id, ParamValue valueNormalized) = 0;
	virtual tresult PLUGIN_API endEdit (ParamID id) = 0;
	virtual tresult PLUGIN_API restartComponent (int32 flags) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IComponentHandler, 0x93A0BEA3, 0x0BD045DB, 0x8E890B0C, 0xC1E46AC6)

//----------

class IComponentHandler2
: public FUnknown {
public:
	virtual tresult PLUGIN_API setDirty (TBool state) = 0;
	virtual tresult PLUGIN_API requestOpenEditor (FIDString name = /*ViewType::*/kEditor) = 0;
	virtual tresult PLUGIN_API startGroupEdit () = 0;
	virtual tresult PLUGIN_API finishGroupEdit () = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IComponentHandler2, 0xF040B4B3, 0xA36045EC, 0xABCDC045, 0xB4D5A2CC)

//----------

class IPlugFrame;

class IPlugView
: public FUnknown {
public:
	virtual tresult PLUGIN_API isPlatformTypeSupported (FIDString type) = 0;
	virtual tresult PLUGIN_API attached (void* parent, FIDString type) = 0;
	virtual tresult PLUGIN_API removed () = 0;
	virtual tresult PLUGIN_API onWheel (float distance) = 0;
	virtual tresult PLUGIN_API onKeyDown (char16 key, int16 keyCode, int16 modifiers) = 0;
	virtual tresult PLUGIN_API onKeyUp (char16 key, int16 keyCode, int16 modifiers) = 0;
	virtual tresult PLUGIN_API getSize (ViewRect* size) = 0;
	virtual tresult PLUGIN_API onSize (ViewRect* newSize) = 0;
	virtual tresult PLUGIN_API onFocus (TBool state) = 0;
	virtual tresult PLUGIN_API setFrame (IPlugFrame* frame) = 0;
	virtual tresult PLUGIN_API canResize () = 0;
	virtual tresult PLUGIN_API checkSizeConstraint (ViewRect* rect) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IPlugView, 0x5BC32507, 0xD06049EA, 0xA6151B52, 0x2B755B29)

//----------

class IPlugFrame
: public FUnknown {
public:
	virtual tresult PLUGIN_API resizeView(IPlugView* view, ViewRect* newSize) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IPlugFrame, 0x367FAF01, 0xAFA94693, 0x8D4DA2A0, 0xED0882A3)

//----------

#define setState setEditorState
#define getState getEditorState

class IEditController
: public IPluginBase {
public:
	virtual tresult     PLUGIN_API setComponentState (IBStream* state) = 0;
	virtual tresult     PLUGIN_API setState (IBStream* state) = 0;
	virtual tresult     PLUGIN_API getState (IBStream* state) = 0;
	virtual int32       PLUGIN_API getParameterCount () = 0;
	virtual tresult     PLUGIN_API getParameterInfo (int32 paramIndex, ParameterInfo& info /*out*/) = 0;
	virtual tresult     PLUGIN_API getParamStringByValue (ParamID id, ParamValue valueNormalized /*in*/, String128 string /*out*/) = 0;
	virtual tresult     PLUGIN_API getParamValueByString (ParamID id, TChar* string /*in*/, ParamValue& valueNormalized /*out*/) = 0;
	virtual ParamValue  PLUGIN_API normalizedParamToPlain (ParamID id, ParamValue valueNormalized) = 0;
	virtual ParamValue  PLUGIN_API plainParamToNormalized (ParamID id, ParamValue plainValue) = 0;
	virtual ParamValue  PLUGIN_API getParamNormalized (ParamID id) = 0;
	virtual tresult     PLUGIN_API setParamNormalized (ParamID id, ParamValue value) = 0;
	virtual tresult     PLUGIN_API setComponentHandler (IComponentHandler* handler) = 0;
	virtual IPlugView*  PLUGIN_API createView (FIDString name) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IEditController, 0xDCD7BBE3, 0x7742448D, 0xA874AACC, 0x979C759E)

#undef setState
#undef getState

//----------

class IEditController2
: public FUnknown {
public:
	virtual tresult PLUGIN_API setKnobMode (KnobMode mode) = 0;
	virtual tresult PLUGIN_API openHelp (TBool onlyCheck) = 0;
	virtual tresult PLUGIN_API openAboutBox (TBool onlyCheck) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IEditController2, 0x7F4EFE59, 0xF3204967, 0xAC27A3AE, 0xAFB63038)

//----------

class ITimerHandler
: public FUnknown {
public:
	virtual void PLUGIN_API onTimer () = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (ITimerHandler, 0x10BDD94F, 0x41424774, 0x821FAD8F, 0xECA72CA9)

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class IHostApplication
: public FUnknown {
public:
	virtual tresult PLUGIN_API getName (String128 name) = 0;
	virtual tresult PLUGIN_API createInstance (TUID cid, TUID _iid, void** obj) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IHostApplication, 0x58E595CC, 0xDB2D4969, 0x8B6AAF8C, 0x36A664E5)

//----------

class IEventHandler
: public FUnknown {
public:
	virtual void PLUGIN_API onFDIsSet (FileDescriptor fd) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IEventHandler, 0x561E65C9, 0x13A0496F, 0x813A2C35, 0x654D7983)

//----------

class IRunLoop
: public FUnknown {
public:
	virtual tresult PLUGIN_API registerEventHandler (IEventHandler* handler, FileDescriptor fd) = 0;
	virtual tresult PLUGIN_API unregisterEventHandler (IEventHandler* handler) = 0;
	virtual tresult PLUGIN_API registerTimer (ITimerHandler* handler, TimerInterval milliseconds) = 0;
	virtual tresult PLUGIN_API unregisterTimer (ITimerHandler* handler) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IRunLoop, 0x18C35366, 0x97764F1A, 0x9C5B8385, 0x7A871389)

//----------

class IParamValueQueue
: public FUnknown {
public:
	virtual ParamID PLUGIN_API getParameterId () = 0;
	virtual int32   PLUGIN_API getPointCount () = 0;
	virtual tresult PLUGIN_API getPoint (int32 index, int32& sampleOffset /*out*/, ParamValue& value /*out*/) = 0;
	virtual tresult PLUGIN_API addPoint (int32 sampleOffset, ParamValue value, int32& index /*out*/) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IParamValueQueue, 0x01263A18, 0xED074F6F, 0x98C9D356, 0x4686F9BA)

//----------

class IParameterChanges
: public FUnknown {
public:
	virtual int32             PLUGIN_API getParameterCount () = 0;
	virtual IParamValueQueue* PLUGIN_API getParameterData (int32 index) = 0;
	virtual IParamValueQueue* PLUGIN_API addParameterData (const ParamID& id, int32& index /*out*/) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IParameterChanges, 0xA4779663, 0x0BB64A56, 0xB44384A8, 0x466FEB9D)

//----------

class IEventList
: public FUnknown {
public:
	virtual int32   PLUGIN_API getEventCount () = 0;
	virtual tresult PLUGIN_API getEvent (int32 index, Event& e /*out*/) = 0;
	virtual tresult PLUGIN_API addEvent (Event& e /*in*/) = 0;
	static const FUID iid;
};

DECLARE_CLASS_IID (IEventList, 0x3A2C4214, 0x346349FE, 0xB2C4F397, 0xB9695A44)

//----------------------------------------------------------------------
#endif

