#ifndef QU_H_
#define QU_H_

#include <QObject>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QFileInfo>

#define N_A "-"
#define NONE "(none)"

#define CHAR_UTF8_APPROX "\xe2\x89\x88"
#define CHAR_UTF8_NEQUAL "\xe2\x89\xa0"

// used for time comparison and fixing
#define LOWER_TIME_BOUND_DEFAULT 15
#define UPPER_TIME_BOUND_DEFAULT 30

//! This class provides global flags and enumerations.
class QU: QObject {
	Q_OBJECT
public:
	QU(QObject *parent = 0);

	enum EventMessageType {
		information,
		warning,
		help,
		saving
	};
	Q_DECLARE_FLAGS(EventMessageTypes, EventMessageType)

	enum PreparatoryTaskMode {
		autoAssignFiles,
		removeUnsupportedTags,
		fixAudioLength
	};
	Q_DECLARE_FLAGS(PreparatoryTaskModes, PreparatoryTaskMode)

	enum CleanTaskMode {
		unusedFiles,
		invalidFileTags,
		removeEndTag
	};
	Q_DECLARE_FLAGS(CleanTaskModes, CleanTaskMode)

	enum FilterMode {
		informationTags = 0x01,
		fileTags = 0x02,
		controlTags = 0x04,
		customTags = 0x08,
		negateFilter = 0x10
	};
	Q_DECLARE_FLAGS(FilterModes, FilterMode)

	enum ScriptableTaskType {
		audioTagTask,
		renameTask
	};
	Q_DECLARE_FLAGS(ScriptableTaskTypes, ScriptableTaskType)

	enum ImageSize {
		mediumImage,
		largeImage
	};
	Q_DECLARE_FLAGS(ImageSizes, ImageSize)

	enum SpellState {
		spellingOk,
		spellingWarning,
		spellingError
	};
	Q_DECLARE_FLAGS(SpellStates, SpellState)

	enum ReportOption {
		reportPrependCurrentPath = 0x01,
		reportPrependUserData = 0x02,
		reportAppendLyrics = 0x04,
		reportLinkLyrics = 0x08,
		reportUseStyleSheet = 0x10
	};
	Q_DECLARE_FLAGS(ReportOptions, ReportOption)

	static QStringList allowedSongFiles();
	static QStringList allowedLicenseFiles();
	static QStringList allowedMidiFiles();
	static QStringList allowedKaraokeFiles();

	static QStringList allowedAudioFiles();
	static QStringList allowedPictureFiles();
	static QStringList allowedVideoFiles();
	static QStringList allowedPlaylistFiles();

	static QString withoutUnsupportedCharacters (const QString &text);
	static QString withoutFolderTags(const QString &text);
	static QString withoutLeadingBlanks(const QString &text);
	static QString withoutAnyUmlaut(const QString &text);
	static QString withoutAnyUmlautEx(const QString &text);

	static bool equal(QString token1, QString token2, bool ignoreEmpty = false);

	static bool fileTypeLessThan(const QFileInfo &fi1, const QFileInfo &fi2);
};

#endif /*QU_H_*/
