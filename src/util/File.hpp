/**
 * \file File.hpp
 *
 * Header des suites de classes pour abstraire les fichiers
 * et les dossiers, en suivant le pattern Decorator
 */

#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <memory>
#include <sys/stat.h>
#include <vector>
#include <map>

using namespace std;

/**
 * \namespace khanar
 *
 * Namespace du projet
 */
namespace khanar
{
    class File;

    typedef bool (*FileSortStrategy)(File const& a, File const& b);  ///< Critère de tri pour les fichiers a et b

    /**
    * \class FileObserver
    *
    * Interface du pattern Observer pour observer la mise à jour des fichiers
    * Ici, le sujet est File
    */
    class FileObserver
    {
      public:
        virtual void fileUpdated(File* file) = 0;
    };

    /**
    * \class FileType
    *
    * Classe servant à représenter un type de fichier : constitué d'un nom et d'un icône
    */
    class FileType
    {
      private:
        string _name;
        string _icon;

      public:
        FileType(string n, string i);

        string getName();
        string getIcon();

    };

    static FileType FILETYPE_DIRECTORY = FileType("Dossier", "folder");
    static FileType FILETYPE_EXECUTABLE = FileType("Fichier exécutable", "application-x-executable");
    static FileType FILETYPE_GENERIC_FILE = FileType("Fichier", "text-x-generic");


    #define FILETYPE_IMAGE(E) FileType(string("Image ") + string(E), "image-x-generic")
    #define FILETYPE_FONT(E) FileType(string("Police ") + string(E), "font-x-generic")
    #define FILETYPE_DOCUMENT(E) FileType(string("Document ") + string(E), "x-office-document")
    #define FILETYPE_VIDEO(E) FileType(string("Vidéo ") + string(E), "video-x-generic")
    #define FILETYPE_AUDIO(E) FileType(string("Fichier son ") + string(E), "video-x-generic")

    static map<string, FileType> FILETYPE_EXTENSIONS_MAP = {
      //Images
      {"png", FILETYPE_IMAGE("PNG")},
      {"jpg", FILETYPE_IMAGE("JPG")},
      {"jpeg", FILETYPE_IMAGE("JPG")},
      {"gif", FILETYPE_IMAGE("GIF")},
      {"bmp", FILETYPE_IMAGE("Bitmap")},
      {"tiff", FILETYPE_IMAGE("TIFF")},
      {"tif", FILETYPE_IMAGE("TIFF")},
      {"raw", FILETYPE_IMAGE("RAW")},

      //Polices
      {"ttf", FILETYPE_FONT("TTF")},
      {"otf", FILETYPE_FONT("OTF")},
      {"woff", FILETYPE_FONT("WOFF")},

      //Documents textes et office
      {"pdf", FILETYPE_DOCUMENT("PDF")},
      {"odt", FILETYPE_DOCUMENT("ODT")},
      {"odp", FILETYPE_DOCUMENT("ODP")},
      {"ods", FILETYPE_DOCUMENT("ODS")},
      {"rtf", FILETYPE_DOCUMENT("RTF")},
      {"xls", FILETYPE_DOCUMENT("Excel")},
      {"xlsx", FILETYPE_DOCUMENT("Excel")},
      {"doc", FILETYPE_DOCUMENT("Word")},
      {"docx", FILETYPE_DOCUMENT("Word")},
      {"ppt", FILETYPE_DOCUMENT("Powerpoint")},
      {"pptx", FILETYPE_DOCUMENT("Powerpoint")},
      {"txt", FILETYPE_DOCUMENT("Texte")},

      //Vidéos
      {"mkv", FILETYPE_VIDEO("MKV")},
      {"mp4", FILETYPE_VIDEO("MP4")},
      {"flv", FILETYPE_VIDEO("Flash")},
      {"avi", FILETYPE_VIDEO("AVI")},
      {"wmv", FILETYPE_VIDEO("WMV")},

      //Sons
      {"mp3", FILETYPE_AUDIO("MP3")},
      {"wav", FILETYPE_AUDIO("WAV")},
      {"aif", FILETYPE_AUDIO("AIFF")},
      {"aiff", FILETYPE_AUDIO("AIFF")},
      {"flac", FILETYPE_AUDIO("FLAC")},
      {"wma", FILETYPE_AUDIO("WMA")},
      {"ogg", FILETYPE_AUDIO("OGG")},
      {"mid", FILETYPE_AUDIO("MIDI")},
      {"midi", FILETYPE_AUDIO("MIDI")},
    }; ///< Mapping entre les extensions et le type de fichier correspondant

    enum Permission
    {
      USR_R = 0,
      USR_W,
      USR_X,

      GRP_R,
      GRP_W,
      GRP_X,

      OTH_R,
      OTH_W,
      OTH_X
    }; ///< Liste des différentes permissions possibles

    /**
     * \class File
     *
     * Wrapper autour des fonctions de manipulation de fichiers
     * de C/C++
     */
    class File
    {
        //TODO Patron Observateur pour notifier des changements du fichier à l'interface

        private:
           string _name;
           string _absolutePath;
           string _parentFolderAbsolutePath;
           string _extension;

           FileSortStrategy _sortStrategy;

           vector<File> _subFiles;
           bool _subFilesCreated;

           struct stat _fileStat; ///< "Statistiques" du fichier, par stat.h (contient des données utiles)
           bool _exists; ///< Défini par stat() et lors de la suppression/création du fichier à la main

           bool _sortDescending;

           void updateStat();
           void updateSubFiles();
           void sortSubFiles();

           void updateAttributes(string absolutepath);

           vector<FileObserver*> _observers;

           void notifyObservers();

           static void updateFavorites(vector<File> newFavorites);

        public:
           /**
           * \brief Constructeur prenant le fichier parent et le nom du fichier actuel
           * \param[in] parent le fichier parent
           * \param[in] name le nom du fichier actuel
           */
           File(File* parent, string name);

           /**
           * \brief Constructeur prenant le chemin absolu du fichier
           * \param[in] string le chemin absolu du fichier
           */
           File(string absolutepath);

           inline bool operator==(const File& other)
           {
             return getAbsolutePath() == other.getAbsolutePath();
           };

           string getName() const;  ///< Récupère le nom du fichier
           string getParentFolderAbsolutePath() const; ///< Récupère le chemin absolu vers le dossier parent
           string getAbsolutePath() const; ///< Récupère le chemin absolu vers le fichier (parentAbsolutePath + name)
           string getExtension() const; ///< Récupère l'extension du fichier
           FileType getFileType() const; ///< Récupère le type du fichier (type de données + icône)
           long getLastAccessTime() const; ///< Récupère la date du dernier accès
           long getLastModificationTime() const; ///< Récupère la date de la dernière modification
           uid_t getUID() const; ///< Récupère l'utilisateur du fichier
           gid_t getGID() const; ///< Récupère le groupe du fichier

           bool isDirectory() const; ///< Indique si le fichier est un dossier
           bool exists() const; ///< Renvoie si le fichier existe ou non
           bool isHidden() const; ///< Renvoie si le fichier est caché ou non (commence par un '.')

           void setName(string newname); ///< Renomme le fichier (le nom rename n'a pas pu être utilisé à cause d'un conflit avec le rename de cstdio)
           void move(string newpath); ///< Déplace/renomme le fichier
           File copy(string newpath) const; ///< Copie le fichier dans newpath et renvoie son objet File
           void removeFile(); ///< Supprime le fichier/dossier (récursivement) (le nom remove n'a pas pu être utilisé à cause d'un conflit avec le rename de cstdio)
           void createNewFile(mode_t mode); ///< Si le fichier n'existe pas, le crée (fichier vide) ainsi que son chemin
           void setGID(gid_t const& gid); ///< Modifie le GID du fichier
           void setUID(uid_t const& uid); ///< Modifie le UID du fichier
           vector<gid_t> getGroupList(); ///< Renvoie la liste des groupes de l'utilisateur du fichier

           void openXterm() const; ///< Si c'est un dossier, ouvre Xterm dans un nouveau processus

           bool getPermission(enum Permission const& perm) const; ///< Renvoie si la permission demandée est accordée ou non (depuis l'enum Permission)
           void setPermission(enum Permission const& perm, bool const& value); ///< Modifie la permission (depuis l'enum Permission)
           bool isExecutable() const; ///< Renvoie si le fichier est exécutable (si il y a la permission X sur usr ou grp ou oth)

           long getSize() const; ///< Renvoie la taille du fichier en octets (ou -1 si c'est un dossier)
           string getFormattedSize() const; ///< Renvoie la taille du fichier formattée dans un String

           vector<File>* getSubFiles(); ///< Si le fichier est un dossier, renvoie la liste des sous dossiers, NULL sinon
           vector<File> search(string expression); ///< Si le fichier est un dossier, recherche (non récursivement) les fichiers correspondant à cette expression

           void setSortStrategy(FileSortStrategy const& strategy, bool const& descending); ///< Change la stratégie de tri des fichiers du dossier ; une stratégie ici est un critère de tri et un ordre

           void subscribeObserver(FileObserver* observer); ///< Abonne un Observer à ce fichier
           void unsubscribeObserver(FileObserver* observer); ///< Désabonne un Observer à ce fichier

           void write(string data); ///< Ecrit dans le fichier (remplace le contenu déjà existant)
           string read(); ///< Lit le contenu du fichier

           bool isInFavorites(); ///< Indique si le fichier est dans les favoris
           void addToFavorites(); ///< Ajoute le fichier aux favoris
           void removeFromFavorites(); ///< Retire le fichier des favoris

           static string FAVORITES_DIRECTORY;
           static vector<File> getFavorites(); ///< Renvoie les dossiers favoris (~/.config/khanar/favorites.json)

           //Stratégies de tri
           static bool NAME_FILESORTSTRATEGY(File const& a, File const& b)
           {
             return a.getName() < b.getName();
           }  ///< Tri alphabétique par nom

           static bool SIZE_FILESORTSTRATEGY(File const& a, File const& b)
           {
             return a.getName() < b.getName();
           }  ///< Tri alphabétique par taille

           static bool ACCESSTIME_FILESORTSTRATEGY(File const& a, File const& b)
           {
             return a.getLastAccessTime() < b.getLastAccessTime();
           }  ///< Tri alphabétique par date de modification

           static string getUIDName(uid_t const& gid); ///< Renvoie le nom d'un UID donné
           static string getGIDName(gid_t const& gid); ///< Renvoie le nom d'un GID donné

           static vector<File> getMountedVolumes(); ///< Renvoie la liste des volumes montés sur le système (volume monté dans la racine exclu)

           static vector<File> getRecentFiles(); ///< Renvoie la liste des fichiers récemment ouverts
    };

    /**
    * \class FileException
    *
    * Classe servant à reporter des erreurs sur les fichiers (entrée/sortie, opérations impossibles...)
    */
    class FileException
    {
      private:
        string _description;

      public:
        /**
        * \brief Constructeur de l'exception
        * \param[in] description la description de l'erreur
        */
        FileException(string description);

        string getDescription(); ///< Renvoie la description de l'exception
    };
}

#endif
