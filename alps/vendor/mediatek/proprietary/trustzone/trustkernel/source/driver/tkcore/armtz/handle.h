#ifndef HANDLE_H
#define HANDLE_H

struct handle_db {
	void **ptrs;
	unsigned max_ptrs;
};

#define HANDLE_DB_INITIALIZER { NULL, 0 }

/*
 * Frees all internal data structures of the database, but does not free
 * the db pointer. The database is safe to reuse after it's destroyed, it
 * just be empty again.
 */
void handle_db_destroy(struct handle_db *db);

/*
 * Allocates a new handle and assigns the supplied pointer to it,
 * ptr must not be NULL.
 * The function returns
 * >= 0 on success and
 * -1 on failure
 */
int handle_get(struct handle_db *db, void *ptr);

/*
 * Deallocates a handle. Returns the associated pointer of the handle
 * the the handle was valid or NULL if it's invalid.
 */
void *handle_put(struct handle_db *db, int handle);

/*
 * Returns the associated pointer of the handle if the handle is a valid
 * handle.
 * Returns NULL on failure.
 */
void *handle_lookup(struct handle_db *db, int handle);

#endif /*HANDLE_H*/
