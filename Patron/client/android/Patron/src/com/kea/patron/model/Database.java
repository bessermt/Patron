// Copyright (c) 2014 Mitch T. Besser, All rights reserved.

package com.kea.patron.model;

import com.kea.patron.PatronApp;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.provider.BaseColumns;

/**
 * @author bessermt
 * 
 */
public class Database {
	public class DbContract extends DbSchemaGen {}

	private class OpenHelper extends SQLiteOpenHelper {
		public static final int DATABASE_VERSION = DbContract.databaseVersion;
		public static final String DATABASE_NAME = DbContract.databaseName;

		public OpenHelper(Context context) {
			super(context, DATABASE_NAME, null, DATABASE_VERSION);
		}

		@Override
		public void onCreate(SQLiteDatabase db) {
			db.execSQL(DbContract.Organization.sqlCreateTable);
			db.execSQL(DbContract.Staff.sqlCreateTable);
			db.execSQL(DbContract.Collection.sqlCreateTable);
			db.execSQL(DbContract.Item.sqlCreateTable);
			if (!db.isReadOnly()) {
				db.execSQL("PRAGMA foreign_keys=ON;"); // Enable foreign key constraints.
			}
		}

		@Override
		public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
			// This database is only a cache for online data, so its upgrade
			// policy is to simply to discard the data and start over.
			db.execSQL(DbContract.Item.sqlDropTable);
			db.execSQL(DbContract.Collection.sqlDropTable);
			db.execSQL(DbContract.Staff.sqlDropTable);
			db.execSQL(DbContract.Organization.sqlDropTable);
			onCreate(db);
		}

		@Override
		public void onDowngrade(SQLiteDatabase db, int oldVersion,
				int newVersion) {
			onUpgrade(db, oldVersion, newVersion);
		}
	}

	private SQLiteDatabase db_;

	private interface Record {
		long getId();
	}

	private abstract class Table<R extends Record> {

		private static final String EXACT_MATCH = "=?";

		protected abstract String getTableName();

		protected abstract ContentValues createContentValues(final R record);

		public long create(final R record) {
			final ContentValues values = createContentValues(record);
			final String tableName = getTableName();

			// Insert the new row, returning the primary key.
			final long newRowId = db_.insert(tableName, null, values);

			return newRowId;
		}

		public Cursor read(final String[] projection, final String selection, final String[] selectionArgs) {

			final String tableName = getTableName();
			final String sortOrder = BaseColumns._ID + " DESC";

			final Cursor cursor = db_.query(
				tableName, 
				projection, 
				selection, 
				selectionArgs, 
				null, 
				null, 
				sortOrder 
			);
			return cursor;
		}

		public int update(final R record) {
			final String tableName = getTableName();
			final ContentValues values = createContentValues(record);

			final String selection = BaseColumns._ID + EXACT_MATCH;
			final String idStr = String.valueOf(record.getId());
			final String[] selectionArgs = { idStr };

			final int count = db_.update(
				tableName,
				values,
				selection,
				selectionArgs
			);
			return count;
		}

		public void delete(final long id) {
			final String tableName = getTableName();
			final String selection = BaseColumns._ID + EXACT_MATCH;
			final String[] selectionArgs = { String.valueOf(id) };
			db_.delete(tableName, selection, selectionArgs);
		}
	}

	public final class Organization extends Table<Organization.Record> {

		@Override
		protected String getTableName() {
			return DbContract.Organization.tableName;
		}

		public class Record extends DbSchemaGen.Organization.Record implements Database.Record {
			@Override
			public long getId() {
				return _id;
			}
		}

		private final class ColumnName extends DbContract.Organization.ColumnName {}

//		// Sample Projection for read():
//		public final String[] projection = 
//		{
//			ColumnName._id,
//			ColumnName.name,
//			ColumnName.type,
//			ColumnName.address,
//			ColumnName.tagline,
//			ColumnName.description,
//			ColumnName.sponsor,
//			ColumnName.phone,
//			ColumnName.web,
//			ColumnName.email,
//			ColumnName.sms,
//			ColumnName.facebook,
//			ColumnName.twitter,
//			ColumnName.blog,
//			ColumnName.socialMedia,
//			ColumnName.gps,
//			ColumnName.icon,
//			ColumnName.logo,
//			ColumnName.pictureGallery,
//			ColumnName.audio,
//			ColumnName.video,
//			ColumnName.feature,
//			ColumnName.calendar,
//			ColumnName.sponsorList,
//			ColumnName.donations,
//			ColumnName.notes,
//			ColumnName.data,
//			ColumnName.url,
//			ColumnName._data
//		};

		@Override
		protected ContentValues createContentValues(final Record record) {

			final ContentValues result = new ContentValues();

			// result.put(ColumnName._id,            record._id);
			result.put(ColumnName.name,           record.name);
			result.put(ColumnName.type,           record.type);
			result.put(ColumnName.address,        record.address);
			result.put(ColumnName.tagline,        record.tagline);
			result.put(ColumnName.description,    record.description);
			result.put(ColumnName.sponsor,        record.sponsor);
			result.put(ColumnName.phone,          record.phone);
			result.put(ColumnName.web,            record.web);
			result.put(ColumnName.email,          record.email);
			result.put(ColumnName.sms,            record.sms);
			result.put(ColumnName.facebook,       record.facebook);
			result.put(ColumnName.twitter,        record.twitter);
			result.put(ColumnName.blog,           record.blog);
			result.put(ColumnName.socialMedia,    record.socialMedia);
			result.put(ColumnName.gps,            record.gps);
			result.put(ColumnName.icon,           record.icon);
			result.put(ColumnName.logo,           record.logo);
			result.put(ColumnName.pictureGallery, record.pictureGallery);
			result.put(ColumnName.audio,          record.audio);
			result.put(ColumnName.video,          record.video);
			result.put(ColumnName.feature,        record.feature);
			result.put(ColumnName.calendar,       record.calendar);
			result.put(ColumnName.sponsorList,    record.sponsorList);
			result.put(ColumnName.donations,      record.donations);
			result.put(ColumnName.notes,          record.notes);
			result.put(ColumnName.data,           record.data);
			result.put(ColumnName.url,            record.url);
			result.put(ColumnName._data,          record._data);

			return result;
		}
	}

	public final class Staff extends Table<Staff.Record> {

		@Override
		protected String getTableName() {
			return DbContract.Staff.tableName;
		}

		public class Record extends DbSchemaGen.Staff.Record implements Database.Record {
			@Override
			public long getId() {
				return _id;
			}
		}

		private final class ColumnName extends DbContract.Staff.ColumnName {}

//		// Sample Projection for read():
//		final String[] projection = {
//			ColumnName._id,
//			ColumnName.organizationId,
//			ColumnName.name,
//			ColumnName.title,
//			ColumnName.description,
//			ColumnName.photo,
//			ColumnName.web,
//			ColumnName.email,
//			ColumnName.sms,
//			ColumnName.facebook,
//			ColumnName.twitter,
//			ColumnName.blog,
//			ColumnName.audio,
//			ColumnName.video,
//			ColumnName.socialMedia,
//			ColumnName.data,
//			ColumnName.url,
//			ColumnName._data
//		};

		@Override
		protected ContentValues createContentValues(final Record record) {

			final ContentValues result = new ContentValues();

			// result.put(ColumnName._id,            record._id);
			result.put(ColumnName.organizationId, record.organizationId);
			result.put(ColumnName.name,           record.name);
			result.put(ColumnName.title,          record.title);
			result.put(ColumnName.description,    record.description);
			result.put(ColumnName.photo,          record.photo);
			result.put(ColumnName.web,            record.web);
			result.put(ColumnName.email,          record.email);
			result.put(ColumnName.sms,            record.sms);
			result.put(ColumnName.facebook,       record.facebook);
			result.put(ColumnName.twitter,        record.twitter);
			result.put(ColumnName.blog,           record.blog);
			result.put(ColumnName.audio,          record.audio);
			result.put(ColumnName.video,          record.video);
			result.put(ColumnName.socialMedia,    record.socialMedia);
			result.put(ColumnName.data,           record.data);
			result.put(ColumnName.url,            record.url);
			result.put(ColumnName._data,          record._data);

			return result;
		}
	}

	public final class Collection extends Table<Collection.Record> {

		@Override
		protected String getTableName() {
			return DbContract.Collection.tableName;
		}

		class Record extends DbSchemaGen.Collection.Record implements Database.Record {
			@Override
			public long getId() {
				return _id;
			}
		}

		private final class ColumnName extends DbContract.Collection.ColumnName {}

//		// Sample Projection for read():
//		final String[] projection = 
//		{
//			ColumnName._id,
//			ColumnName.organizationId,
//			ColumnName.collectionId,
//			ColumnName.name,
//			ColumnName.description,
//			ColumnName.sponsor,
//			ColumnName.location,
//			ColumnName.web,
//			ColumnName.audio,
//			ColumnName.video,
//			ColumnName.data,
//			ColumnName.url,
//			ColumnName._data
//		};

		@Override
		protected ContentValues createContentValues(final Record record) {

			final ContentValues result = new ContentValues();

			result.put(ColumnName._id,            record._id);
			result.put(ColumnName.organizationId, record.organizationId);
			result.put(ColumnName.collectionId,   record.collectionId);
			result.put(ColumnName.name,           record.name);
			result.put(ColumnName.description,    record.description);
			result.put(ColumnName.sponsor,        record.sponsor);
			result.put(ColumnName.location,       record.location);
			result.put(ColumnName.web,            record.web);
			result.put(ColumnName.audio,          record.audio);
			result.put(ColumnName.video,          record.video);
			result.put(ColumnName.data,           record.data);
			result.put(ColumnName.url,            record.url);
			result.put(ColumnName._data,          record._data);

			return result;
		}
	}

	public final class Item extends Table<Item.Record> {

		@Override
		protected String getTableName() {
			return DbContract.Item.tableName;
		}

		class Record extends DbSchemaGen.Item.Record implements Database.Record {
			@Override
			public long getId() {
				return _id;
			}
		}

		private final class ColumnName extends DbContract.Item.ColumnName {}

//		// Sample Projection for read():
//		final String[] projection = 
//		{
//			ColumnName._id,
//			ColumnName.organizationId,
//			ColumnName.collectionId,
//			ColumnName.name,
//			ColumnName.description,
//			ColumnName.sponsor,
//			ColumnName.photo,
//			ColumnName.diagram,
//			ColumnName.location,
//			ColumnName.like,
//			ColumnName.adopt,
//			ColumnName.web,
//			ColumnName.audio,
//			ColumnName.video,
//			ColumnName.data,
//			ColumnName.url,
//			ColumnName._data
//		};

		@Override
		protected ContentValues createContentValues(final Record record) {

			final ContentValues result = new ContentValues();

			result.put(ColumnName._id,            record._id);
			result.put(ColumnName.organizationId, record.organizationId);
			result.put(ColumnName.collectionId,   record.collectionId);
			result.put(ColumnName.name,           record.name);
			result.put(ColumnName.description,    record.description);
			result.put(ColumnName.sponsor,        record.sponsor);
			result.put(ColumnName.photo,          record.photo);
			result.put(ColumnName.diagram,        record.diagram);
			result.put(ColumnName.location,       record.location);
			result.put(ColumnName.like,           record.like);
			result.put(ColumnName.adopt,          record.adopt);
			result.put(ColumnName.web,            record.web);
			result.put(ColumnName.audio,          record.audio);
			result.put(ColumnName.video,          record.video);
			result.put(ColumnName.data,           record.data);
			result.put(ColumnName.url,            record.url);
			result.put(ColumnName._data,          record._data);

			return result;
		}
	}

	public Database(final Context context) {
		final OpenHelper openHelper = new OpenHelper(context);
		db_ = openHelper.getWritableDatabase();
	}

	public Database() {
		this(PatronApp.getAppContext());
	}

	public void close() {
		db_.close();
	}
}
