#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <spawn.h>

#if !defined(REPOSITORY)
# define REPOSITORY "https://raw.githubusercontent.com/fenze/rose-packages/main/"
#endif

void die(const char *msg)
{
	perror(msg);
	exit(1);
}

size_t write_callback(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

int fetchinfo(const char *package)
{
	CURL *curl;
	CURLcode res;

	char *url = strdup(REPOSITORY);
	url = realloc(url, strlen(url) + strlen(package) + 8);
	strcat(url, package);
	strcat(url, "/package");

	FILE *output = fopen("/tmp/mini", "wb");
	if (!output) {
		perror("fopen");
		return 1;
	}

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, output);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fputs("cannot do request", stderr);
			fclose(output);
			curl_easy_cleanup(curl);
			return 1;
		}

		curl_easy_cleanup(curl);
	} else {
		fprintf(stderr, "failed to initialize libcurl\n");
		fclose(output);
		return 1;
	}

	fclose(output);

	output = fopen("/tmp/mini", "r");

	char buffer[4];
	memset(&buffer, 0, 4);
	fgets(buffer, 2, output);

	if (*buffer == '4') {
		fprintf(stderr, "[%s]: package not found\n", package);
		remove("/tmp/mini");
		return 1;
	}

	return 0;
}

int install(int argc, char *const *restrict argv)
{
	if (argc == 0) {
		fputs("expected package name\n", stderr);
		return 1;
	}

	if (*argv[0] == '-') {
		if (argv[0][1] == 'h') {
			puts("usage: mini -i [PACKAGE...]");
			return 0;
		} else {
			fputs("invalid operation specified\n", stderr);
			return 1;
		}
	}

	for (int i = 0; i < argc; i++) {
		if (fetchinfo(argv[i]) != 0)
			return 1;

		pid_t pid;
		char *const args[] = { "install", argv[i], NULL };
		int ret = posix_spawn(&pid, "/usr/lib/mini/install", NULL, NULL, args, NULL);

		if (pid == -1)
			die("posix_spawn");
	}

	return 0;
}

int uninstall(int argc, char *const *restrict argv)
{
	if (argc == 0) {
		puts("expected package name");
		return 1;
	}

	if (*argv[0] == '-') {
		if (argv[0][1] == 'h') {
			puts("usage: mini -r [PACKAGE...]");
			return 0;
		} else {
			puts("invalid operation specified");
			return 1;
		}
	}

	for (int i = 0; i < argc; i++) {
		pid_t pid;
		char *const args[] = { "remove", argv[i], NULL };
		int ret = posix_spawn(&pid, "/usr/lib/mini/remove", NULL, NULL, args, NULL);

		if (pid == -1)
			die("posix_spawn");
	}

	return 0;
}

int update(int argc, char *const *restrict argv)
{
	if (argc == 0) {
		puts("expected package name");
		return 1;
	}

	if (*argv[0] == '-') {
		if (argv[0][1] == 'h') {
			puts("usage: mini -u [PACKAGE...]");
			return 0;
		} else {
			puts("invalid operation specified");
			return 1;
		}
	}

	for (int i = 0; i < argc; i++) {
		if (fetchinfo(argv[i]) != 0)
			return 1;

		pid_t pid;
		char *const args[] = { "update", argv[i], NULL };
		int ret = posix_spawn(&pid, "/usr/lib/mini/update", NULL, NULL, args, NULL);

		if (pid == -1)
			die("posix_spawn");
	}

	return 0;
}

int main(int argc, char **argv)
{
	if (getuid() != 0) {
		fputs("must be run as root\n", stderr);
		return 1;
	}

	if (--argc == 0) {
		fputs("no operation specified", stderr);
		return 1;
	}

	switch (((++argv)[0][0] == '-') ? argv[0][1] : argv[0][0]) {
		case 'i':
			return install(--argc, ++argv);
		case 'u':
			return update(--argc, ++argv);
		case 'r':
			return uninstall(--argc, ++argv);
		case 'h':
			puts("usage: mini [-iruh] [package...]");
			return 0;
		case 'v':
			puts("mini v0.1-alpha");
			return 0;
		default:
			fputs("invalid operation specified", stderr);
			return 1;
	}

	return 0;
}
