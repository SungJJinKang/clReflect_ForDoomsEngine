
//
// ===============================================================================
// clReflect
// -------------------------------------------------------------------------------
// Copyright (c) 2011-2012 Don Williamson & clReflect Authors (see AUTHORS file)
// Released under MIT License (see LICENSE file)
// ===============================================================================
//

#include "Database.h"

#include <stdlib.h>
#include <assert.h>


namespace
{
	cldb::u32 CalcFieldHash(const cldb::Field& field, bool considerIsConst)
	{
		// Construct the fully-qualified type name and hash that
		std::string name;
		if(considerIsConst == true)
		{
			//KSJ
			name += field.qualifier.is_const ? "const " : "";
		}
		
		name += field.type.text;
		name += field.qualifier.op == cldb::Qualifier::POINTER ? "*" : field.qualifier.op == cldb::Qualifier::REFERENCE ? "&" : "";
		return clcpp::internal::HashNameString(name.c_str());
	}
}


cldb::u32 cldb::CalculateFunctionUniqueID(const std::vector<Field>& parameters)
{
	// Mix with all parameter field hashes
	cldb::u32 unique_id = 0;
	for (size_t i = 0; i < parameters.size(); i++)
	{
		bool considerConst = false; // KSH
		if(i == 0) // KSH
		{
			if(parameters[0].name.text == "this")
			{//if parameter is this paramter ( member function )
				considerConst = true;
			}
		} // KSH
		// MapFilePaser::UndecorateFunctionSignature doesn't show const of function's paramter
		// So If we calculate unique id of function with const specifier, it can make problem.
		// because clscan know function paramter's is const, but clexport::MapFilePaser don't

		cldb::u32 field_hash = CalcFieldHash(parameters[i], considerConst);
		unique_id = clcpp::internal::MixHashes(unique_id, field_hash);
	}

	return unique_id;
}


cldb::Database::Database()
{
}


void cldb::Database::AddBaseTypePrimitives()
{
	// Create a selection of basic C++ types
	// TODO: Figure the size of these out based on platform
	Name parent;
	AddPrimitive(Type(GetName("void"), parent, 0));
	AddPrimitive(Type(GetName("bool"), parent, sizeof(bool)));
	AddPrimitive(Type(GetName("char"), parent, sizeof(char)));
	AddPrimitive(Type(GetName("unsigned char"), parent, sizeof(unsigned char)));
	AddPrimitive(Type(GetName("wchar_t"), parent, sizeof(wchar_t)));
	AddPrimitive(Type(GetName("short"), parent, sizeof(short)));
	AddPrimitive(Type(GetName("unsigned short"), parent, sizeof(unsigned short)));
	AddPrimitive(Type(GetName("int"), parent, sizeof(int)));
	AddPrimitive(Type(GetName("unsigned int"), parent, sizeof(unsigned int)));
	AddPrimitive(Type(GetName("long"), parent, sizeof(long)));
	AddPrimitive(Type(GetName("unsigned long"), parent, sizeof(unsigned long)));
	AddPrimitive(Type(GetName("float"), parent, sizeof(float)));
	AddPrimitive(Type(GetName("double"), parent, sizeof(double)));

	// 64-bit types as clang sees them
	AddPrimitive(Type(GetName("long long"), parent, sizeof(clcpp::int64)));
	AddPrimitive(Type(GetName("unsigned long long"), parent, sizeof(clcpp::uint64)));
}


void cldb::Database::AddContainerInfo(const std::string& container, const std::string& read_iterator, const std::string& write_iterator, bool has_key)
{
	ContainerInfo ci;
	ci.name = GetName(container.c_str());
	ci.read_iterator_type = GetName(read_iterator.c_str());
	ci.write_iterator_type = GetName(write_iterator.c_str());
	ci.flags = has_key ? ContainerInfo::HAS_KEY : 0;
	m_ContainerInfos[ci.name.hash] = ci;
}


cldb::Name cldb::Database::AddTypeInheritance(const Name& derived_type, const Name& base_type)
{
	std::string text = base_type.text + "<-" + derived_type.text;
	TypeInheritance ti;
	ti.name = GetName(text.c_str()); 
	ti.derived_type = derived_type;
	ti.base_type = base_type;
	m_TypeInheritances[ti.name.hash] = ti;

	return ti.name;
}


const cldb::Name& cldb::Database::GetName(const char* text)
{
	// Check for nullptr and empty string representations of a "noname"
	thread_local static Name noname;
	if (text == 0)
	{
		return noname;
	}
	u32 hash = clcpp::internal::HashNameString(text);
	if (hash == 0)
	{
		return noname;
	}

	// See if the name has already been created
	NameMap::iterator i = m_Names.find(hash);
	if (i != m_Names.end())
	{
		// Check for collision
		assert(i->second.text == text && "Hash collision!");
		return i->second;
	}

	// Add to the database
	i = m_Names.insert(NameMap::value_type(hash, Name(hash, text))).first;
	return i->second;
}


const cldb::Name& cldb::Database::GetName(u32 hash) const
{
	// Check for DB existence first
	NameMap::const_iterator i = m_Names.find(hash);
	if (i == m_Names.end())
	{
		thread_local static Name noname;
		return noname;
	}
	return i->second;
}
