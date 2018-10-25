/* src/include/config.h.  Generated from config.h.in by configure.  */
///
/// @file config.h
///
/// @brief Configuração para o aplicativo.
///
/// Gerado automaticamente pelo processo ./configure esse arquivo descreve
/// as configurações de ambiente detectadas automaticamente.
///
/// @author perry.werneck@gmail.com
///
///

#ifndef CONFIG_H_INCLUDED

	#define CONFIG_H_INCLUDED 1

	/// @brief Nome do pacote.
	#define PACKAGE_NAME "pdf2db"

	/// @brief Versão atual.
	#define PACKAGE_VERSION "0.1"

 	/// @brief Descrição do aplicativo.
	#define PACKAGE_DESCRIPTION "PDF to Database parser"

	/// @brief Tenho ICONV?
	#define HAVE_ICONV 1

	/// @brief Indica se é possível usar traduções
	#define HAVE_LIBINTL 1

	/// @brief Indica se o pugixml está disponível
	#define HAVE_PUGIXML

	/// @brief Define to 1 if the system has getopt()
	#define HAVE_GETOPT 1

	/// @brief Defined on most unixes
	#define HAVE_UNISTD_H 1

#endif // CONFIG_H_INCLUDED

