//
// Created by seedship on 4/11/21.
//

#ifndef UAVGS_IGETTABLEJSONOBJECT_H
#define UAVGS_IGETTABLEJSONOBJECT_H

#include <QJsonObject>

class IGettableJsonObject
{
public:
	virtual QJsonObject
	get() const = 0;
};

#endif //UAVGS_IGETTABLEJSONOBJECT_H
