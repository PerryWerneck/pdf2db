/**
 * @file src/main/contents.cc
 *
 * @brief Implementa extratores de conteúdo.
 *
 * @author perry.werneck@gmail.com
 *
 * $URL$
 * $Revision$ $Author$
 *
 */

 #include <pdf2db.h>
 #include <climits>
 #include <regex>

/*---[ Implement ]----------------------------------------------------------------------------------*/

 PDFImporter::Property::Property(const XMLNode &node) {
 	this->name = node.attribute("name").as_string();
	this->page = node.attribute("page").as_uint(0);
	this->line = node.attribute("line").as_uint(0);

	this->range.from = node.attribute("start-char").as_uint(1);
	if(this->range.from == 0) {
		throw EINVAL;
	}

	this->range.length = node.attribute("line-length").as_uint(UINT_MAX);

 }

 void PDFImporter::Property::prepare(std::vector<string> &text) {

	for(auto line = text.begin(); line != text.end(); line++) {

		if(this->range.from != 0 || this->range.length != UINT_MAX) {
			string nl = line->substr(this->range.from-1,std::min((line->size() - (this->range.from-1)),this->range.length));
			debug("%s:\n%s\n%s",this->name.c_str(),line->c_str(),nl.c_str());
			line->assign(nl);
		}

		strip(*line);

	}


 }

 PDFImporter::Property * PDFImporter::Property::create(const XMLNode &node) {

	/// @brief Tipo da propriedade que será criada.
	auto type = node.attribute("type").as_string("regex");

	/// @brief Elemento para extrair um valor com base numa expressão regular.
	class RegexExtractedProperty : public Property {
	private:
		std::regex expression;

	public:
		RegexExtractedProperty(const XMLNode &node) : Property(node) {
			expression = node.attribute("expression").as_string();
		}

		virtual ~RegexExtractedProperty() {
		}

		void set(std::vector<string> &text) override {

			std::smatch match;

			for(auto line : text) {

				if(std::regex_search(line, match, this->expression)) {

					this->value = match.str(1).c_str();
					strip(this->value);
					if(PDFImporter::verbose) {
						cout << getName() << "=\"" << this->value << "\"" << endl;
					}
					break;

				}

			}

		}

	};

	if(!strcasecmp(type,"regex")) {
		return new RegexExtractedProperty(node);
	}

	/// @brief Elemento para extrair um bloco de texto.
	class RegexBlockExtractor : public Property {
	private:
		std::regex	from;
		std::regex	to;

	public:

		RegexBlockExtractor(const XMLNode &node) : Property(node) {
			from = node.attribute("begin").as_string();
			to = node.attribute("end").as_string();
		}

		virtual ~RegexBlockExtractor() {
		}

		void set(std::vector<string> &text) override {

			bool loading = false;

			for(auto line : text) {

				if(loading) {

					if(regex_match(line,to)) {
						loading = false;
						debug("%s: Achei final (%s)",getName().c_str(),line.c_str());
						return;
					}

					debug("[%s]",line.c_str());
					if(!this->value.empty()) {
						this->value += "\n";
					}
					this->value += line;

				} else if(regex_match(line,from)) {
					debug("%s: Achei inicio (%s)",getName().c_str(),line.c_str());
					loading = true;
				}

			};

		}

	};

	if(!strcasecmp(type,"text-block")) {
		return new RegexBlockExtractor(node);
	}

	/// @brief Elemento para extrair um texto.
	class TextExtractor : public Property {
	private:

	public:

		TextExtractor(const XMLNode &node) : Property(node) {
		}

		virtual ~TextExtractor() {
		}

		void set(std::vector<string> &text) override {

			for(auto line : text) {

				if(!this->value.empty()) {
					this->value += "\n";
				}

				this->value += line;

			};

		}

	};

	if(!strcasecmp(type,"text")) {
		return new TextExtractor(node);
	}

	// Não consegui criar, erro.
 	throw EINVAL;

 }



