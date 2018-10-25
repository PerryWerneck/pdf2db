/**
 * @file pdfimporter/src/include/pdfimporter.h
 *
 * @brief Definições para o importador de PDF C++
 *
 * @author perry.werneck@gmail.com
 *
 * $URL$
 * $Revision$ $Author$
 *
 */

#ifndef PDFIMPORPER_H_INCLUDED

	#define PDFIMPORPER_H_INCLUDED 1

	#include <config.h>
	#include <components/core/defs.h>
	#include <components/core/log.h>
	#include <components/core/string.h>
	#include <functional>
	#include <pugixml.hpp>
	#include <cppdb/frontend.h>

	using APPNAME::string;
	using std::vector;
	using XMLNode = pugi::xml_node;

	namespace PDFImporter {

		/// @brief Documento PDF já convertido para texto sem espaços.
		class Document {
		private:

			class Page : public std::vector<string> {
			private:
				friend class PDFImporter::Document;

			public:
				Page() = default;

			};

			std::vector<Page> pages;

		public:
			Document(const char *name);

			bool forEach(std::function<bool(const char *line)> callback) const;

			const string & getString(unsigned int page, unsigned int line) const;

			inline const char * get(unsigned int page, unsigned int line) const {
				return getString(page,line).c_str();
			}


		};

		/// @brief Filtro a aplicar num documento.
		class Filter {
		protected:
			Filter() = default;

		public:

			/// @brief Cria filtro de acordo com a descrição XML.
			static Filter * create(const XMLNode &node);

			virtual ~Filter() {};

			/// @brief Verifica se o documento atende o filtro.
			virtual bool test(const Document &document) = 0;

		};

		/// @brief Valor extraido do documento.
		class Content {
		private:

			/// @brief Nome da propriedade
			string name;

		protected:
			Content(const XMLNode &node);

			/// @brief De que página será extraída.
			unsigned int page;

		public:

			virtual ~Content() { }

			/// @brief Cria conteúdo de acordo com a descrição XML.
			static Content * create(const XMLNode &node);

			/// @brief Extrai conteúdo do documento.
			virtual bool set(const Document &document) = 0;

			/// @brief Reinicia o parser.
			virtual void reset() = 0;

			/// @brief Obtêm o nome do valor
			const string & getName() const {
				return this->name;
			}

		};

		/// @brief Comando SQL já preparado.
		class Query {
		private:
			/// @brief Statement a enviar para o banco.
			cppdb::statement st;

			/// @brief Nome das propriedades para o SQL.
			std::vector<string> names;

		public:
			Query(cppdb::session &sql, const XMLNode &node);
			~Query();

		};

		/// @brief Parser de documento.
		class Parser {
		private:

			/// @brief Lista de filtros a aplicar no documento.
			std::vector<Filter *> filters;

			/// @brief Lista de valores extraídos do documento..
			std::vector<Content *> contents;

			/// @brief Comandos SQL a executar.
			std::vector<Query *> queryes;

		public:

			Parser(cppdb::session &sql, const XMLNode &node);
			~Parser();

			/// @brief Faz o parse do documento.
			bool set(const Document &document);

		};


	}


#endif // PDFIMPORPER_H_INCLUDED
