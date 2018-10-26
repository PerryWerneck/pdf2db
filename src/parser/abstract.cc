/**
 * @file src/parser/abstract.cc
 *
 * @brief
 *
 * @author perry.werneck@gmail.com
 *
 * $URL$
 * $Revision$ $Author$
 *
 */

 #include <pdf2db.h>
 #include <cstring>

/*---[ Implement ]----------------------------------------------------------------------------------*/

 PDFImporter::Parser::Abstract::Abstract(cppdb::session &sql, const XMLNode &node, const Abstract *parent) {

 	this->parent = parent;

  	for(auto property = node.child("property"); property; property = property.next_sibling("property")) {
		properties.push_back(Property::create(property));
  	}

  	for(auto query = node.child("sql"); query; query = query.next_sibling("sql")) {
		queryes.push_back(new Query(sql,query));
  	}

 }

 PDFImporter::Parser::Abstract::~Abstract() {

	for(auto child : childs) {
		delete child;
	}

	for(auto query : this->queryes) {
		delete query;
	}

	for(auto property : properties) {
		delete property;
	}

 }

 const char * PDFImporter::Parser::Abstract::operator [](const char *name) const {

	for(auto property : properties) {

		if(!strcasecmp(property->getName().c_str(),name)) {
			return property->c_str();
		}

	}

	if(parent) {
		return parent->operator[](name);
	}

 	return "";

 }

 void PDFImporter::Parser::Abstract::store(cppdb::session &sql) {

	for(auto query : queryes) {
		query->exec(sql, *this);
	}

	for(auto child : childs) {
		child->store(sql);
	}

 }



