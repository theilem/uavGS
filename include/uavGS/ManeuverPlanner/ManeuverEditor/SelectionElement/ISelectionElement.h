//
// Created by seedship on 4/5/21.
//

#ifndef UAVGS_ISELECTIONOBJECT_H
#define UAVGS_ISELECTIONOBJECT_H

template<class T>
class ISelectionElement
{
public:
//	virtual ISelectionElement(const std::string& name, QWidget *parent = nullptr) = 0;
	virtual std::pair<std::string, T> get() = 0;

	inline std::string makeTitle(const std::string& title, int level)
	{
		std::string ans;
		for(int x = 0; x < level; x++)
		{
			ans += "\t";
		}
		return ans + title;
	}
};

#endif //UAVGS_ISELECTIONOBJECT_H
