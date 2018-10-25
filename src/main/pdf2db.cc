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
#include <cstring>

using namespace std;

/*---[ Implement ]----------------------------------------------------------------------------------*/

/// @brief Diretório de onde serão lidas as definições de importação.
static const char *defdir = "./import.d";

/// @brief URL para acesso ao banco de dados
static const char *dburi = "mysql:user='ctpetUI';password='ctPET@27903';database='ctpet';protocol='socket';unix_socket='/var/run/mysql/mysql.sock'";

/// @brief Verifica se o arquivo termina em .xml
static int xmlFilter(const struct dirent *entry) {
	return hasSuffix(entry->d_name,".xml") ? 1 : 0;
}

int main(int argc, const char *argv[]) {

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

		if(parser->set(sql, document)) {
			cout << "Encontrei documento válido" << endl;
			break;
		}

	}

	return 0;

}


/**
 * @brief Removes trailing whitespace from a string.
 *
 * This function doesn't allocate or reallocate any memory;
 * it modifies in place. Therefore, it cannot be used
 * on statically allocated strings.
 *
 * Reference: <https://git.gnome.org/browse/glib/tree/glib/gstrfuncs.c>
 *
 * @see chug() and strip().
 *
 * @return pointer to string.
 *
 */
char * chomp(char *str) noexcept {

	size_t len = strlen(str);

	while(len--) {

		if(isspace(str[len])) {
			str[len] = 0;
		} else {
			break;
		}
	}

	return str;

}

/**
 * @brief Remove the leading whitespace from the string.
 *
 * Removes leading whitespace from a string, by moving the rest
 * of the characters forward.
 *
 * This function doesn't allocate or reallocate any memory;
 * it modifies the string in place. Therefore, it cannot be used on
 * statically allocated strings.
 *
 * Reference: <https://git.gnome.org/browse/glib/tree/glib/gstrfuncs.c>
 *
 * @see chomp() and strip().
 *
 * @return pointer to string.
 *
 */
char * chug (char *str) noexcept {

	char *start;

	for (start = (char*) str; *start && isspace(*start); start++);

	memmove(str, start, strlen ((char *) start) + 1);

	return str;
}

char * strip(char *str) noexcept {
	return chomp(chug(str));
}

string & strip(string &s) noexcept {
	char *str = new char[s.size()+1];
	memcpy(str,s.c_str(),s.size());
	str[s.size()] = 0;
	strip(str);
	s.assign(str);
	delete[] str;
	return s;
}

/**
 * @brief Looks whether the string ends with suffix.
 *
 * Reference: <https://git.gnome.org/browse/glib/tree/glib/gstrfuncs.c>
 *
 * Obs: The test is case insensitive.
 *
 */
bool hasSuffix(const char *str, const char *suffix) noexcept {

	int str_len     = strlen(str);
	int suffix_len  = strlen(suffix);

	if (str_len < suffix_len)
		return false;

	return strcasecmp ( (str + str_len) - suffix_len, suffix) == 0;

}

