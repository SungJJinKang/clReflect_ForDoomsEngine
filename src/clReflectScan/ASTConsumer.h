
//
// ===============================================================================
// clReflect, ASTConsumer.h - Traversal of the clang AST for C++, returning an
// offline Reflection Database.
// -------------------------------------------------------------------------------
// Copyright (c) 2011-2012 Don Williamson & clReflect Authors (see AUTHORS file)
// Released under MIT License (see LICENSE file)
// ===============================================================================
//

#include "clReflectCore/Database.h"
#include <clang/Basic/SourceManager.h>

class ReflectionSpecs;

namespace clang
{
    class ASTContext;
    class ASTRecordLayout;
    class NamedDecl;
    class TranslationUnitDecl;
    class QualType;
    struct PrintingPolicy;
}

class ASTConsumer
{
public:
    ASTConsumer(cldb::Database& db, const ReflectionSpecs& rspecs, const std::string& ast_log);

    void WalkTranlationUnit(clang::ASTContext* ast_context, clang::TranslationUnitDecl* tu_decl);

    cldb::Database& GetDB()
    {
        return m_DB;
    }
    clang::ASTContext& GetASTContext()
    {
        return *m_ASTContext;
    }
    const ReflectionSpecs& GetReflectionSpecs()
    {
        return m_ReflectionSpecs;
    }
	const std::map<std::string, std::vector<cldb::Primitive*>>& GetSourceFilePathOfDeclMap()
	{
		return m_SourceFilePathOfDeclMap;
	}
	std::map <cldb::u32, cldb::u32>& GetTypeInheritanceDeclararingOrder()
	{
		return m_TypeInheritanceDeclararingOrder;
	}

	void AddSourceLocation(const clang::SourceLocation& sourceLocation, cldb::Primitive* primitive);

private:
    void AddDecl(clang::NamedDecl* decl, const std::string& parent_name, const clang::ASTRecordLayout* layout);
    void AddNamespaceDecl(clang::NamedDecl* decl, const std::string& name, const std::string& parent_name);
    void AddClassDecl(clang::NamedDecl* decl, const std::string& name, const std::string& parent_name);
    void AddEnumDecl(clang::NamedDecl* decl, const std::string& name, const std::string& parent_name);
    void AddFunctionDecl(clang::NamedDecl* decl, const std::string& name, const std::string& parent_name);
    void AddMethodDecl(clang::NamedDecl* decl, const std::string& name, const std::string& parent_name);
    void AddFieldDecl(clang::NamedDecl* decl, const std::string& name, const std::string& parent_name,
                      const clang::ASTRecordLayout* layout);
    void AddClassTemplateDecl(clang::NamedDecl* decl, const std::string& name, const std::string& parent_name);
    void AddContainedDecls(clang::NamedDecl* decl, const std::string& parent_name, const clang::ASTRecordLayout* layout);

    void MakeFunction(clang::NamedDecl* decl, const std::string& function_name, const std::string& parent_name,
                      std::vector<cldb::Field>& parameters);

	

    cldb::Database& m_DB;

    clang::ASTContext* m_ASTContext;

    const ReflectionSpecs& m_ReflectionSpecs;

	// key : SourceFilePath of Decl 
	// implemented 100%
	std::map <std::string , std::vector<cldb::Primitive*>> m_SourceFilePathOfDeclMap;
	std::map <cldb::u32, cldb::u32> m_TypeInheritanceDeclararingOrder;

    bool m_AllowReflect;
};
