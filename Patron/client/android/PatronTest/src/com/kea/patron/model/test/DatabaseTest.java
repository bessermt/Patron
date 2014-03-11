/**
 * 
 */
package com.kea.patron.model.test;

import android.database.Cursor;
import android.test.AndroidTestCase;
import android.test.RenamingDelegatingContext;

import com.kea.patron.model.Database;

/**
 * @author bessermt
 *
 */
public class DatabaseTest extends AndroidTestCase {

	private Database db_;

	public DatabaseTest() {
		super();
	}

	/* (non-Javadoc)
	 * @see android.test.ActivityInstrumentationTestCase2#setUp()
	 */
	@Override
	protected void setUp() throws Exception {
		super.setUp();
		RenamingDelegatingContext context = new RenamingDelegatingContext(getContext(), "test_");
		db_ = new Database(context);
	}

	private long addOrganization() {
		final Database.Organization organizationTable = db_.new Organization();
		final Database.Organization.Record organizationRecord = organizationTable.new Record();
		organizationRecord.name = "Grizzly and Wolf Discovery Center";
		organizationRecord.type = "Zoo";
		organizationRecord.address = "201 S. Canyon St., West Yellowstone, MT 59758";

		final long recordId = organizationTable.create(organizationRecord);

		final class ColumnName extends Database.DbContract.Organization.ColumnName {};
		final String[] projection = {
			ColumnName._id,
			ColumnName.name,
			ColumnName.type,
			ColumnName.address,
			ColumnName.gps
		};

		final Cursor cursor = organizationTable.read(projection, null, null);
		if (cursor == null) {
			assertTrue(false);
		}
		else {
			assertTrue(cursor.getCount() == 1);
			final boolean success = cursor.moveToFirst();
			assertTrue(success);
			if (success) {
				final int idIndex = cursor.getColumnIndexOrThrow(ColumnName._id);
				final int nameIndex = cursor.getColumnIndexOrThrow(ColumnName.name);
				final int typeIndex = cursor.getColumnIndexOrThrow(ColumnName.type);
				final int addressIndex = cursor.getColumnIndexOrThrow(ColumnName.address);
				final int gpsIndex = cursor.getColumnIndexOrThrow(ColumnName.gps);
				while (cursor.isAfterLast() == false) {
					final long id = cursor.getLong(idIndex);
					assertTrue(id == recordId);
					final String name = cursor.getString(nameIndex);
					assertTrue(name.equals(organizationRecord.name));
					final String type = cursor.getString(typeIndex);
					assertTrue(type.equals(organizationRecord.type));
					final String address = cursor.getString(addressIndex);
					assertTrue(address.equals(organizationRecord.address));
					final String gps = cursor.getString(gpsIndex);
					assertTrue(gps == null);
					cursor.moveToNext();
				}
			}
		}
		return recordId;
	}

	private long addStaff(final long organizationId) {
		final Database.Staff staffTable = db_.new Staff();
		final Database.Staff.Record staffRecord = staffTable.new Record();
		staffRecord.organizationId = organizationId;
		staffRecord.name = "Joan Embery";
		staffRecord.title = "Keeper";
		staffRecord.description = "Joan enjoys sharing her knowledge of the animals in her care. If you are lucky enough to spot Joan, be sure introduce yourself and ask her your questions regarding care of the zoo's animals.";

		final long recordId = staffTable.create(staffRecord);
		assertTrue(recordId > 0);

		final class ColumnName extends Database.DbContract.Staff.ColumnName {}
		final String[] projection = {
			ColumnName._id,
			ColumnName.name,
			ColumnName.title,
			ColumnName.description,
			ColumnName.photo
		};

		final Cursor cursor = staffTable.read(projection, null, null);
		if (cursor == null) {
			assertTrue(false);
		}
		else {
			assertTrue(cursor.getCount() == 1);
			final boolean success = cursor.moveToFirst();
			if (success) {
				final int idIndex = cursor.getColumnIndexOrThrow(ColumnName._id);
				final int nameIndex = cursor.getColumnIndexOrThrow(ColumnName.name);
				final int titleIndex = cursor.getColumnIndexOrThrow(ColumnName.title);
				final int descriptionIndex = cursor.getColumnIndexOrThrow(ColumnName.description);
				final int photoIndex = cursor.getColumnIndexOrThrow(ColumnName.photo);
				while (cursor.isAfterLast() == false) {
					final long id = cursor.getLong(idIndex);
					assertTrue(id == recordId);
					final String name = cursor.getString(nameIndex);
					assertTrue(name.equals(staffRecord.name));
					final String title = cursor.getString(titleIndex);
					assertTrue(title.equals(staffRecord.title));
					final String description = cursor.getString(descriptionIndex);
					assertTrue(description.equals(staffRecord.description));
					final String photo = cursor.getString(photoIndex);
					assertTrue(photo == null);
					cursor.moveToNext();
				}
			}
		}
		return recordId;
	}

	// @Test
	public void testAdd() {
		final long organizationId = addOrganization();
		assertTrue(organizationId > 0);
		final long staffId = addStaff(organizationId);
		assertTrue(staffId > 0);
	}

	@Override
	public void tearDown() throws Exception {
		super.tearDown();
		db_.close();
	}
}
