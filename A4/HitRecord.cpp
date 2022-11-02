#include "HitRecord.hpp"


std::ostream & operator << (std::ostream & os, const HitRecord & record) {

	//os << "SceneNode:[NodeType: ___, name: ____, id: ____, isSelected: ____, transform: ____"
	os << "HitRecord:[m_t: "<<record.m_t<<"\n";
    os << "m_position: "<<record.m_position.x<<" "<<record.m_position.y<<" "<<record.m_position.z<<"\n";
    os << "m_normal: "<<record.m_normal.x<<" "<<record.m_normal.y<<" "<<record.m_normal.z<<"]\n";

	return os;
}