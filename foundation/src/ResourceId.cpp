#include "ResourceId.h"
using mh::foundation::ResourceId;

ResourceId::ResourceId(const std::string& text) :
	text_(text)
{}

bool ResourceId::operator<(const ResourceId& other) const
{
	return this->text_ < other.text_;
}

bool ResourceId::operator==(const ResourceId& other) const
{
	return this->text_ == other.text_;
}

const std::string& ResourceId::filename() const
{
	return this->text_;
}
