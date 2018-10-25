/**
 * @file src/main/query.cc
 *
 * @brief Implementa operações SQL.
 *
 * @author perry.werneck@gmail.com
 *
 * $URL$
 * $Revision$ $Author$
 *
 */

 #include <pdfimporter.h>

/*---[ Implement ]----------------------------------------------------------------------------------*/

 PDFImporter::Query::Query(cppdb::session &sql, const pugi::xml_node &node) {

 	string str = node.child_value();

 	str.strip();

 	size_t from;
 	while( (from = str.find_first_of("${")) != string::npos ) {
		size_t to = str.find_first_of("}",from);
		if(to == string::npos) {
			throw EINVAL;
		}

		size_t szBlock = (to - from);
		names.emplace_back(str.c_str()+from+2,(szBlock)-2);

		debug("key=\"%s\"",names.back().c_str());
		str.replace(from,szBlock+1,"?");


 	}

 	debug("SQL:\n%s\n",str.c_str());
 	st= sql.prepare(str.c_str());

 }

 PDFImporter::Query::~Query() {

 }
