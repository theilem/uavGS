//
// Created by seedship on 4/9/21.
//

#ifndef UAVGS_IOVERRIDESPROVIDER_H
#define UAVGS_IOVERRIDESPROVIDER_H

class IOverridesProvider
{
public:
	virtual const std::vector<std::string>&
	getOverrides() const = 0;

	virtual const std::vector<std::string>&
	getMaintains() const = 0;
};

#endif //UAVGS_IOVERRIDESPROVIDER_H
