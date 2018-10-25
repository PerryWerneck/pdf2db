/**
 * @file src/pdfimporter.cc
 *
 * @brief Ponto de entrada para o aplicativo de importação de exames em PDF
 *
 * @author perry.werneck@gmail.com
 *
 * Referências:
 *
 * <https://stackoverflow.com/questions/2732178/extracting-text-from-pdf-with-poppler-c>
 * <http://marpirk.github.io/poppler-cpp-doc/classpoppler_1_1document.html>
 *
 */

#include <pdf2db.h>
#include <iostream>
#include <vector>
#include <dirent.h>

using namespace std;

/*---[ Implement ]----------------------------------------------------------------------------------*/

/// @brief Diretório de onde serão lidas as definições de importação.
static const char *defdir = "./import.d";

/// @brief URL para acesso ao banco de dados
static const char *dburi = "mysql:user='ctpetUI';password='ctPET@27903';database='ctpet';protocol='socket';unix_socket='/var/run/mysql/mysql.sock'";

/// @brief Verifica se o arquivo termina em .xml
static int xmlFilter(const struct dirent *entry) {
	return 1;
//	return APPNAME::string::hasSuffix(entry->d_name,".xml") ? 1 : 0;
}

int main(int argc, const char *argv[]) {

	PDFImporter::Document("./sample.pdf").forEach([](const char *line) {
		cout << line << endl;
		return true;
	});


/*
	/// @brief Lista de parsers definida
	std::vector<PDFImporter::Parser> parsers;

	/// @brief Conexão com o banco de dados.
	cppdb::session sql(dburi);

	// Carrega definições de procedimentos
	struct dirent **namelist;
	int qtdFiles = scandir(defdir, &namelist, xmlFilter, alphasort);
	if(qtdFiles < 0) {
		throw errno;
	}

	for(int file = 0; file < qtdFiles; file++) {

		string filename = defdir;
		filename += "/";
		filename += namelist[file]->d_name;
		free(namelist[file]);

		clog << "Lendo definições de " << filename << "..." << endl;

		pugi::xml_document doc;
		doc.load_file(filename.c_str());

		for(XMLNode top : doc) {

			for(XMLNode node = top.child("procedure"); node; node = node.next_sibling("procedure")) {
				parsers.emplace_back(sql,node);
			}

		}


	}

	free(namelist);

	cout << "Carreguei " << parsers.size() << " formatos de documento." << endl;

	// Carrega arquivos PDF.
	PDFImporter::Document document("./sample.pdf");

#ifdef DEBUG
	size_t ln = 1;
	document.forEach([&ln](const char *line) {
		cout << (ln++) << ":\t" << line << endl;
		return true;
	});
#endif // DEBUG

	for(auto parser = parsers.begin(); parser != parsers.end(); parser++) {

		if(parser->set(document)) {
			cout << "Encontrei documento válido" << endl;
			break;
		}

	}
*/

	return 0;

}


