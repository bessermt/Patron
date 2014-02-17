/*
 * Copyright (C) 2014 Mitch Besser
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// TODO: #include <string.h>
// TODO: #include <jni.h>

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
jstring Java_com_example_hellojni_HelloJni_stringFromJNI(JNIEnv* env,
		jobject thiz) {
#if defined(__arm__)
#  if defined(__ARM_ARCH_7A__)
#    if defined(__ARM_NEON__)
#      define ABI "armeabi-v7a/NEON"
#    else
#      define ABI "armeabi-v7a"
#    endif
#  else
#   define ABI "armeabi"
#  endif
#elif defined(__i386__)
#   define ABI "x86"
#elif defined(__mips__)
#   define ABI "mips"
#else
#   define ABI "unknown"
#endif

	return (*env)->NewStringUTF(env,
			"Hello from JNI !  Compiled with ABI " ABI ".");
}

// Patron Database:
//
// +--------------+       +--------------+
// | Organization +------<|    Staff     |
// |--------------|       |--------------|
// |              |       |              |
// |              |       |              |
// +------+-------+       +--------------+
//        |       |
//        |       +--------------+
//        |   +-------+          |
//        |   |       |          |
//        |   |       |          |
// +------^---^---+---+   +------^-------+
// |  Collection  +------<|     Item     |
// |--------------|       |--------------|
// |              |       |              |
// |              |       |              |
// +--------------+       +--------------+

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

// App Variables:
// TODO: Decide where app variables should live.
// - Kiosk Mode
// - Coupons
// - etc.

// Table names:
#define TABLE_NAME_ORGANIZATION "Organization"
#define TABLE_NAME_STAFF "Staff"
#define TABLE_NAME_COLLECTION "Collection"
#define TABLE_NAME_ITEM "Item"

// Common table column names:
#define COLUMN_NAME_ID    "_id"    // see Android BaseColumns._ID
#define COLUMN_NAME_COUNT "_count" // see Android BaseColumns._COUNT
#define COLUMN_NAME_DATA  "_data"  // see Android MediaStore.MediaColumns.DATA

// Common SQL command fragments:
#define SQL_CREATE_TABLE "CREATE TABLE "
#define SQL_DROP_TABLE "DROP TABLE IF EXISTS "
#define SQL_COLUMN_TYPE_ID " INTEGER PRIMARY KEY AUTOINCREMENT"
#define SQL_COLUMN_TYPE_REQUIRED " TEXT NOT NULL"
#define SQL_COLUMN_TYPE_OPTIONAL " TEXT"

#define SEQ_ELEM_NAME_SQLTYPE(name, sqltype) ((name)(sqltype))

#define SEQ_ELEM_NAME(col_name_sqltype_seq_elem) BOOST_PP_SEQ_ELEM(0, col_name_sqltype_seq_elem)
#define SEQ_ELEM_SQLTYPE(col_name_sqltype_seq_elem) BOOST_PP_SEQ_ELEM(1, col_name_sqltype_seq_elem)

#define COL_NAME(_r, _data, n, col_name_sqltype_seq_elem) \
    BOOST_PP_COMMA_IF(n)                                   \
    SEQ_ELEM_NAME(col_name_sqltype_seq_elem)

#define COL_NAME_SQLTYPE(_r, _data, n, col_name_sqltype_seq_elem) \
    BOOST_PP_COMMA_IF(n)                                           \
    SEQ_ELEM_NAME(col_name_sqltype_seq_elem)                       \
    SEQ_ELEM_SQLTYPE(col_name_sqltype_seq_elem)

// Macros used to create extra data columns for possible future use:

#define _SQL_EXTRA_COLUMN_NAME_VARS_FOR_EACH_TYPE(r, n, type) \
    ColumnNameType BOOST_PP_CAT(BOOST_PP_CAT(extra, type), n);

#define _SQL_EXTRA_COLUMN_NAME_VARS_REPEAT_COUNT(z, n, type_seq) \
    BOOST_PP_SEQ_FOR_EACH(_SQL_EXTRA_COLUMN_NAME_VARS_FOR_EACH_TYPE, n, type_seq)

#define SQL_EXTRA_COLUMN_NAME_VARS(n, type_seq) \
    BOOST_PP_REPEAT(n, _SQL_EXTRA_COLUMN_NAME_VARS_REPEAT_COUNT, type_seq)

#define _SQL_EXTRA_COLUMN_NAMES_FOR_EACH_TYPE(r, n, type) \
    BOOST_PP_COMMA_IF(n) BOOST_PP_STRINGIZE(BOOST_PP_CAT(BOOST_PP_CAT(extra, type), n))

#define _SQL_EXTRA_COLUMN_NAMES_REPEAT_COUNT(z, n, type_seq) \
    BOOST_PP_COMMA_IF(n) BOOST_PP_SEQ_FOR_EACH(_SQL_EXTRA_COLUMN_NAMES_FOR_EACH_TYPE, n, type_seq)

#define SQL_EXTRA_COLUMN_NAMES(n, type_seq) \
    BOOST_PP_REPEAT(n, _SQL_EXTRA_COLUMN_NAMES_REPEAT_COUNT, type_seq)

#define _SQL_EXTRA_CREATE_FOR_EACH_TYPE(r, n, type) \
    , BOOST_PP_CAT(BOOST_PP_CAT(extra, type), n) type

#define _SQL_EXTRA_CREATE_REPEAT_COUNT(z, n, type_seq) \
    BOOST_PP_SEQ_FOR_EACH(_SQL_EXTRA_CREATE_FOR_EACH_TYPE, n, type_seq)

#define SQL_EXTRA_CREATE(n, type_seq) \
    BOOST_PP_REPEAT(n, _SQL_EXTRA_CREATE_REPEAT_COUNT, type_seq)

// TODO: factor: "4, (TEXT)(INTEGER)(REAL)(BLOB)"

#define SQL_TABLE_INIT(table_name, col_name_sqltype_seq)                  \
    table_name, /* tableName */                                            \
    { /* columnName */                                                     \
        BOOST_PP_SEQ_FOR_EACH_I(COL_NAME, _, col_name_sqltype_seq)         \
        SQL_EXTRA_COLUMN_NAMES(4, (TEXT)(INTEGER)(REAL)(BLOB))             \
    },                                                                     \
    CREATE_TABLE table_name /* sqlCreateTable */                           \
    "("                                                                    \
        BOOST_PP_SEQ_FOR_EACH_I(COL_NAME_SQLTYPE, _, col_name_sqltype_seq) \
        SQL_EXTRA_CREATE(4, (TEXT)(INTEGER)(REAL)(BLOB))                   \
    ");",                                                                  \
    SQL_DROP_TABLE table_name ";" /* sqlDropTable */

// Common datatypes: TODO: Keep these?
typedef static const char ColumnNameType[];
typedef static const long RecordIdType;

static const struct DbContract
{
	const char* const databaseName;
	const int databaseVersion;

	// Android specific.
	// An empty file in external files folder prevents media scanner from seeing files.
	const char* const filenameHideMedia;

	// Common column names:
	const char* const columnNameId; // android.provider.BaseColumns._ID
	const char* const columnNameCount; // android.provider.BaseColumns._COUNT
	const char* const columnNameData; // android.provider.MediaStore.MediaColumns.DATA

	const struct
	{
		const char* const tableName;

		const struct
		{
			ColumnNameType id;
			ColumnNameType name;
			ColumnNameType type; // Zoo, Art Museum, Museum, Aquarium, ...
			ColumnNameType tagline;
			ColumnNameType description;
			ColumnNameType address;
			ColumnNameType phone;
			ColumnNameType web;
			ColumnNameType email;
			ColumnNameType sms;
			ColumnNameType facebook;
			ColumnNameType twitter;
			ColumnNameType blog;
			ColumnNameType socialMedia; // access google+, tumblr, instagram, pinterest, ...
			ColumnNameType gps;
			ColumnNameType icon;
			ColumnNameType logo;
			ColumnNameType pictureGallery;
			ColumnNameType feature;
			ColumnNameType calendar;
			ColumnNameType donations;
			ColumnNameType notes;
			ColumnNameType data;
			ColumnNameType url;

			SQL_EXTRA_COLUMN_NAME_VARS(4, (TEXT)(INTEGER)(REAL)(BLOB))
		} columnName;

		const char* const sqlCreateTable;

		const char* const sqlDropTable;
	} organization;

	const struct
	{
		const char* const tableName;

		const struct
		{
			ColumnNameType id;
			ColumnNameType key_organization_id;
			ColumnNameType name;
			ColumnNameType title;
			ColumnNameType description;
			ColumnNameType photo;
			ColumnNameType web;
			ColumnNameType email;
			ColumnNameType sms;
			ColumnNameType facebook;
			ColumnNameType twitter;
			ColumnNameType blog;
			ColumnNameType socialMedia; // google+, tumblr, instagram, pinterest, ...;
			ColumnNameType data;
			ColumnNameType url;

			SQL_EXTRA_COLUMN_NAME_VARS(4, (TEXT)(INTEGER)(REAL)(BLOB))
		} columnName;

		const char* const sqlCreateTable;

		const char* const sqlDropTable;
	} staff;

	const struct
	{
		const char* const tableName;

		const struct
		{
			ColumnNameType id;
			ColumnNameType key_organization_id;
			ColumnNameType key_collection_id;
			ColumnNameType name;
			ColumnNameType description;
			ColumnNameType location;
			ColumnNameType web;
			ColumnNameType data;
			ColumnNameType url;

			SQL_EXTRA_COLUMN_NAME_VARS(4, (TEXT)(INTEGER)(REAL)(BLOB))
		} columnName;

		const char* const sqlCreateTable;

		const char* const sqlDropTable;
	} collection;

	const struct
	{
		const char* const tableName;

		const struct
		{
			ColumnNameType id;
			ColumnNameType key_organization_id;
			ColumnNameType key_collection_id;
			ColumnNameType name;
			ColumnNameType description;
			ColumnNameType photo;
			ColumnNameType diagram;
			ColumnNameType location;
			ColumnNameType sponsor;
			ColumnNameType like;
			ColumnNameType adopt;
			ColumnNameType web;
			ColumnNameType data;
			ColumnNameType url;

			SQL_EXTRA_COLUMN_NAME_VARS(4, (TEXT)(INTEGER)(REAL)(BLOB))
		} columnName;

		const char* const sqlCreateTable;

		const char* const sqlDropTable;
	} item;
} DbContract =
{
	"Patron.db", // databaseName
	1,          // databaseVersion
	“.nomedia”, // filenameHideMedia
	ID,         // columnNameId
	COUNT,      // columnNameCount
	DATA,       // columnNameData

	{
		SQL_TABLE_INIT
		(
			TABLE_NAME_ORGANIZATION,

			SEQ_ELEM_NAME_SQLTYPE(ID,               SQL_COLUMN_TYPE_ID)
			SEQ_ELEM_NAME_SQLTYPE("name",           SQL_COLUMN_TYPE_REQUIRED)
			SEQ_ELEM_NAME_SQLTYPE("type",           SQL_COLUMN_TYPE_REQUIRED)
			SEQ_ELEM_NAME_SQLTYPE("tagline",        SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("description",    SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("address",        SQL_COLUMN_TYPE_REQUIRED)
			SEQ_ELEM_NAME_SQLTYPE("phone",          SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("web",            SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("email",          SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("sms",            SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("facebook",       SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("twitter",        SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("blog",           SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("socialMedia",    SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("gps",            SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("icon",           SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("logo",           SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("pictureGallery", SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("feature",        SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("calendar",       SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("sponsorList",    SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("donations",      SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("notes",          SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("data",           SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("url",            SQL_COLUMN_TYPE_OPTIONAL)
		)
	},
	{
		SQL_TABLE_INIT
		(
			TABLE_NAME_STAFF,

			SEQ_ELEM_NAME_SQLTYPE(ID,                    SQL_COLUMN_TYPE_ID)
			SEQ_ELEM_NAME_SQLTYPE("key_organization_id", SQL_COLUMN_TYPE_ID)
			SEQ_ELEM_NAME_SQLTYPE("name",                SQL_COLUMN_TYPE_REQUIRED)
			SEQ_ELEM_NAME_SQLTYPE("title",               SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("description",         SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("photo",               SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("web",                 SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("email",               SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("sms",                 SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("facebook",            SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("twitter",             SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("blog",                SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("socialMedia",         SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("data",                SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("url",                 SQL_COLUMN_TYPE_OPTIONAL)
		)
	},
	{
		SQL_TABLE_INIT
		(
			TABLE_NAME_COLLECTION,

			SEQ_ELEM_NAME_SQLTYPE(ID,                    SQL_COLUMN_TYPE_ID)
			SEQ_ELEM_NAME_SQLTYPE("key_organization_id", SQL_COLUMN_TYPE_ID)
			SEQ_ELEM_NAME_SQLTYPE("key_collection_id",   SQL_COLUMN_TYPE_ID)
			SEQ_ELEM_NAME_SQLTYPE("name",                SQL_COLUMN_TYPE_REQUIRED)
			SEQ_ELEM_NAME_SQLTYPE("description",         SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("location",            SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("web",                 SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("data",                SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("url",                 SQL_COLUMN_TYPE_OPTIONAL)
		)
	},
	{
		SQL_TABLE_INIT
		(
			TABLE_NAME_ITEM,

			SEQ_ELEM_NAME_SQLTYPE(ID,                    SQL_COLUMN_TYPE_ID)
			SEQ_ELEM_NAME_SQLTYPE("key_organization_id", SQL_COLUMN_TYPE_ID)
			SEQ_ELEM_NAME_SQLTYPE("key_collection_id",   SQL_COLUMN_TYPE_ID)
			SEQ_ELEM_NAME_SQLTYPE("name",                SQL_COLUMN_TYPE_REQUIRED)
			SEQ_ELEM_NAME_SQLTYPE("description",         SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("photo",               SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("diagram",             SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("location",            SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("sponsor",             SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("like",                SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("adopt",               SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("web",                 SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("data",                SQL_COLUMN_TYPE_OPTIONAL)
			SEQ_ELEM_NAME_SQLTYPE("url",                 SQL_COLUMN_TYPE_OPTIONAL)
		)
	}
};

#undef EXTRA_N_X_TYPES

#undef SQL_EXTRA_CREATE
#undef _SQL_EXTRA_CREATE_REPEAT_COUNT
#undef _SQL_EXTRA_CREATE_FOR_EACH_TYPE

#undef SQL_EXTRA_COLUMN_NAMES
#undef _SQL_EXTRA_COLUMN_NAMES_REPEAT_COUNT
#undef _SQL_EXTRA_COLUMN_NAMES_FOR_EACH_TYPE

#undef SQL_EXTRA_COLUMN_NAME_VARS
#undef _SQL_EXTRA_COLUMN_NAME_VARS_REPEAT_COUNT
#undef _SQL_EXTRA_COLUMN_NAME_VARS_FOR_EACH_TYPE

#undef SQL_TABLE_INIT

#undef COL_NAME_SQLTYPE

#undef COL_NAME

#undef SEQ_ELEM_SQLTYPE
#undef SEQ_ELEM_NAME

#undef SEQ_ELEM_NAME_SQLTYPE

#undef SQL_COLUMN_TYPE_OPTIONAL
#undef SQL_COLUMN_TYPE_REQUIRED
#undef SQL_COLUMN_TYPE_ID
#undef SQL_DROP_TABLE
#undef SQL_CREATE_TABLE

#undef COLUMN_NAME_DATA
#undef COLUMN_NAME_COUNT
#undef COLUMN_NAME_ID

#undef TABLE_NAME_ITEM
#undef TABLE_NAME_COLLECTION
#undef TABLE_NAME_STAFF
#undef TABLE_NAME_ORGANIZATION
