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

 #include <pdf2db.h>

/*---[ Implement ]----------------------------------------------------------------------------------*/

 PDFImporter::Query::Query(cppdb::session &sql, const pugi::xml_node &node) {

 	string str = node.child_value();

#ifdef DEBUG
	this->text = str;
#endif // DEBUG

 	strip(str);

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
 	st = sql.prepare(str.c_str());

 }

 PDFImporter::Query::~Query() {

 }


 /// @brief Armazena valores processados no banco.
 void PDFImporter::Query::store(cppdb::session &sql, const Parser &parser) {

	// Just in case.
	st.reset();

	// Faz o bind dos valores.
	for(size_t ix = 0; ix < names.size(); ix++) {

		try {

			const char *ptr = parser[names[ix].c_str()];

			debug("%03d(%s)=\"%s\"",(int) (ix+1),names[ix].c_str(),ptr);

			st.bind(ix+1,ptr);

		} catch(const std::exception &e) {

			std::cerr << "Can't bind field \"" << names[ix] << "\": " << e.what() << std::endl;
			throw;

		}

	}

	// Executa e limpa para o próximo
	debug("Executing\n%s",text.c_str());
	st.exec();

	debug("%s: Reseting...",__FUNCTION__);
	st.reset();

 }

