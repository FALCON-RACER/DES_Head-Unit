#include "utils.h"
#include <QByteArray>

namespace Utils {

	QString encodeBase64(const QString str)	{
		QByteArray byteArray = str.toUtf8();
		QByteArray encoded = byteArray.toBase64();

		return QString(encoded);
	}
}
