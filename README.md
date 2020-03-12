# SoalShiftSISOP20_modul2_F10

### 1. Soal Satu :

Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan
ketentuan sebagai berikut:

Jawab : 

Fungsi untuk mengecek apakah string yang diinputkan * atau tidak
```Javascript
int isStar(char * str){
	int length = strlen(str);
	if(length == 1){
		if(strcmp(str, "*") == 0)return 1;
	}
	return 0;
}
```
- Panjang dari string yang diinputkan harus 1 (berupa *), sehingga menggunakan ```strlen()``` untuk menghitung panjang string
- Mengecek apakah string yang diinputkan * atau tidak meggunakan ```strcmp()```

Fungsi untuk mengecek apakah string yang diinputkan angka atau tidak
```Javascript
int isNumber(char * str){
	int length = strlen(str);
	for(int i = 0; i < length; i++){
		if(!isdigit(str[i]))
			return 0;
	}
    return 1;
}
```
- Mengetahui panjang dari string yang diinputkan menggunakan ```strlen``` 
- Mengecek apakah setiap angka yang diinputkan berupa digit desimal (0 - 9) atau tidak. Apabila bukan digit desimal, maka tidak dijalankan

Fungsi untuk nge print error
```Javascript
void printError(){
	printf("Invalid Parameter\n");
}
```

Fungsi untuk menetapkan parameter (berupa digit angka dan bintang)
```Javascript
void assignParam(int * num, int min, int max, char * str){
	if(isNumber(str)){
		*num = atoi(str);
		if(*num < min || *num > max){
			printError();
			exit(EXIT_FAILURE);
		}
	}else if(isStar(str)){
		*num = -1;
	}else{
		printError();
		exit(EXIT_FAILURE);
	}
}
```
- Kalau string yang diinputkan berupa angka (di cek menggunakan fungsi ```isNumber```), maka convert string ke integer menggunakan atoi.
- Lalu di cek apakah integer (second, minute, hour) yang diinput lebih dari batas (second min 0, max59; minute min 0, max 59; hour min 0, max 23 ). Apabila melebihi batas, maka ```printError```
- Kalau string yang diinputkan berupa angka (di cek menggunakan fungsi ```isStar```), maka tandai integer jadi -1
- Apabila karakter yang dimasukkan bukan digit angka atau *, maka ```printError```

Main Function
```Javascript
int main(int argc, char ** argv) {
	if(argc != 5){
		printError();
		exit(EXIT_FAILURE);
	}

	char * basharg[] = {"bash", argv[4], NULL};

	int second, minute, hour;
	
 	assignParanm(&second, 0, 59, argv[1]);
 	assignParanm(&minute, 0, 59, argv[2]);
 	assignParanm(&hour, 0, 23, argv[3]);

	if ((chdir("/")) < 0) {
		exit(EXIT_FAILURE);
	}

	FILE * file = fopen(argv[4], "r");
	if(!file){
		printf("Invalid Path\n");
		exit(EXIT_FAILURE);
	}
	pid_t pid, sid;

	pid = fork();

 	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();
	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
```
- Apabila jumlah argumen bukan 5, maka program error, karena argumen 0 tempat program dijalankan, argumen 1 untuk second, argumen 2 untuk minute, argumen 3 untuk hour, dan argumen 4 untuk menjalankan program
- ```char * basharg[]``` untuk menjalankan program
- Masukkan parameter detik, menit, dan jam melalui fungsi  ```assignParam```
- Mengubah working directory ke root menggunakan ```chdir```
- ```FILE * file = fopen(argv[4], "r");``` untuk mengecek apakah program .sh dapat di read / tidak, kalau tidak bs di baca error
- Selanjutnya melakukan langkah-langkah pembuatan proses daemon

```Javascript
while (1) {
		time_t t = time(NULL);
		struct tm * timeNow = localtime(&t);
		int timeToSleep = 0;
		if(hour == -1){

		}else if(timeNow->tm_hour <= hour){
			timeToSleep += hour - timeNow->tm_hour;
		}else{
			timeToSleep += 23 - timeNow->tm_hour + hour;
		}
		timeToSleep *= 60;
		if(minute == -1){

		}else if(timeNow->tm_min <= minute){
			timeToSleep += minute - timeNow->tm_min;
		}else{
			timeToSleep += 59 - timeNow->tm_min + minute;
		}
		timeToSleep *= 60;
		if(second == -1){
			
		}else if(timeNow->tm_sec <= second){
			timeToSleep += second - timeNow->tm_sec;
		}else{
			timeToSleep += 59 - timeNow->tm_sec + second;
		}
		if(timeToSleep == 0)timeToSleep = 1;
		sleep(timeToSleep);
		pid_t pid = fork();
		if(pid < 0)
			exit(EXIT_FAILURE);
		if(pid == 0){
			char dir[100];
			strcpy(dir, argv[4]);
			int dirEndIndex = 0;
			for(int i = strlen(dir); i >= 0; i--){
				if(dir[i] == '/'){
					dirEndIndex = i;
					break;
				}
			}
			dir[dirEndIndex] = '\0';
			chdir(dir);
			execv("/bin/bash", basharg);
		}
```
- Mengecek jam, apabila berupa *, maka dijalankan setiap jam (tidak pengaruh pada delay), jadi tidak menjalankan apa-apa. 
Namun apabila jam berupa digit desimal, maka ada 2 kemungkinan, yaitu digit input lebih dari / kurang dari waktu sekarang 
Apabila digit input kurang dari waktu sekarang, maka delaynya adalah 23 dikurang jam yang sekarang dan di tambah jam selanjutnya dimana perintah mau dieksekusi. 
Misalkan ingin sekarang jam 15.00, dan mau di eksekusi jam 00.00, maka 23 - 15 + 0 = 8 jam delay.
Apabila digit input lebih dari waktu sekarang, maka delaynya jam yang sekarang dan di kurang jam selanjutnya dimana perintah mau dieksekusi. 
Misalkan ingin sekarang jam 15.00, dan mau di eksekusi jam 18.00, maka 18 - 15 = 3 jam delay.
- Mengecek menit, apabila berupa *, maka dijalankan setiap menit (tidak pengaruh pada delay), jadi tidak menjalankan apa-apa. 
Namun apabila menit berupa digit desimal, maka ada 2 kemungkinan, yaitu digit input lebih dari / kurang dari waktu sekarang 
Apabila digit input kurang dari waktu sekarang, maka delaynya adalah 59 dikurang menit yang sekarang dan di tambah menit selanjutnya dimana perintah mau dieksekusi. 
Misalkan ingin sekarang menit ke 45, dan mau di eksekusi pada menit ke 30, maka 59 - 45 + 30 = 44 menit delay.
Apabila digit input lebih dari waktu sekarang, maka delaynya menit yang sekarang dan di kurang menit selanjutnya dimana perintah mau dieksekusi. 
Misalkan ingin sekarang menit ke 45 dan mau di eksekusi di menit ke 50 , maka 50 - 45 = 5 menit delay.
- Mengecek detik, apabila berupa *, maka dijalankan setiap detik (tidak pengaruh pada delay), jadi tidak menjalankan apa-apa. 
Namun apabila detik berupa digit desimal, maka ada 2 kemungkinan, yaitu digit input lebih dari / kurang dari waktu sekarang 
Apabila digit input kurang dari waktu sekarang, maka delaynya adalah 59 dikurang detik yang sekarang dan di tambah detik selanjutnya dimana perintah mau dieksekusi. 
Misalkan ingin sekarang detik ke 45, dan mau di eksekusi pada detik ke 30, maka 59 - 45 + 30 = 44 detik delay.
Apabila digit input lebih dari waktu sekarang, maka delaynya detik yang sekarang dan di kurang detik selanjutnya dimana perintah mau dieksekusi. 
Misalkan ingin sekarang detik ke 45 dan mau di eksekusi di detik ke 50 , maka 50 - 45 = 5 detik delay.
- ```if(timeToSleep == 0)timeToSleep = 1``` digunakan untuk mengset nilai minimum delay yaitu 1 detik
- Buar child process menggunakan ```fork```
- Mengubah directory menggunakan ```chdir()``` agar hasil yang diperoleh akan masuk ke directory yang kita mau
- Eksekusi perintah menggunakan ```execv```

### 2. Soal Dua :



### 3. Soal Tiga :

##### a. Program buatan jaya harus bisa membuat dua direktori di “/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu lima detik kemudian membuat direktori yang kedua bernama “sedaap”.
Jawab : 
```Javascript
	pid_t pid = fork();

	if(pid < 0)
		exit(EXIT_FAILURE);

	if(chdir("/home/geizka") < 0){
		exit(EXIT_FAILURE);
	}

	if(pid == 0){
		pid = fork();
		if(pid < 0)
			exit(EXIT_FAILURE);
		if(pid == 0){
			char * arg[] = {"mkdir", "-p", "./modul2/indomie", NULL};
			execv("/bin/mkdir", arg);
		}
		wait(NULL);
		sleep(5);
		char * arg[] = {"mkdir", "-p", "./modul2/sedaap", NULL};
		execv("/bin/mkdir", arg);
	}
```
- Untuk membuat direktori pertama (indomie), ```fork()``` dulu. Apabila merupakan child process, maka buat direktori menggunakan ```mkdir -p```, lalu jalankan program menggunakan perintah ```execv()```
- ```wait(NULL)``` untuk mendelay proses pembuatan direktori sedaap yang merupakan proses selanjutnya
- Menggunakan perintah ```sleep(5)``` karena direktori sedaap akan dibuat 5 detik setelah direktori indomie dibuat 

##### b. Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori“/home/[USER]/modul2/”. 
Jawab : 
```Javascript
pid = fork();
	if(pid < 0)
		exit(EXIT_FAILURE);
	if(pid == 0){
		char * arg[] = {"unzip", "./modul2/jpg.zip", "-d", "./modul2", NULL};
		execv("/usr/bin/unzip", arg);
	}
	wait(NULL);
```
 - Untuk meng unzip file, ```fork()``` dulu. Apabila merupakan child process, maka unzip jpg menggunakan ```unzip()``` , lalu jalankan program menggunakan perintah ```execv()```
 - Menggunakan system call ```wait()``` untuk mendelay proses pemindahan file / direktori ( proses soal no 3 )

##### c. Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (didalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan pengelompokan, semua file harus dipindahkan ke“/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke “/home/[USER]/modul2/indomie/”.
Jawab :
```Javascript
	struct dirent * de;
	DIR * directory = opendir("./modul2/jpg");
	while((de = readdir(directory)) != NULL){
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			continue;
		char fileName[300];
		char destName[300];
		if(de->d_type == DT_DIR){
			snprintf(fileName, sizeof(fileName), "./modul2/jpg/%s", de->d_name);
			snprintf(destName, sizeof(destName), "./modul2/indomie/%s", de->d_name);
			pid = fork();
			if(pid == 0){
				char * arg[] = {"mv", fileName, destName, NULL};
				execv("/bin/mv", arg);
			}
			else{
			snprintf(fileName, sizeof(fileName), "./modul2/jpg/%s", de->d_name);
			snprintf(destName, sizeof(destName), "./modul2/sedaap/%s", de->d_name);
			pid = fork();
			if(pid == 0){
				char * arg[] = {"mv", fileName, destName, NULL};
				execv("/bin/mv", arg);
			}
			wait(NULL);
```
- ```wait(NULL)``` untuk mendelay proses pembuatan file coba2.txt yang merupakan rposes selanjutnya
-  ```struct dirent * de``` adalah struct untuk membaca file yang ada di direktori
- Perintah akan dijalankan apabila directory tidak kosong, sehingga menggunakan perintah ```readdir(directory)```
- Apabila direktori berupa . dan .., maka direktori tidak termasuk (proses jalan terus / continue), sehingga harus dibandingkan dengan de menggunakan ```strcmp()```
- Apabila tipe de merupakan direktori, maka menggunakan fungsi ```snprintf()``` memformat dan menyimpan nama hasil ekstrak (yang ber tipe direktori) yang mau disimpan di direktori indomie dalam buffer array.
- Memindahkan nama hasil ekstrak (yang ber tipe direktori) ke direktori indomie menggunakan ```mv```, setelah di ```fork()``` terlebih dahulu
- Apabila tipe de merupakan file, maka menggunakan fungsi ```snprintf()``` memformat dan menyimpan nama hasil ekstrak (yang ber tipe file) yang mau disimpan di direktori sedaap dalam buffer array.
- Memindahkan nama hasil ekstrak (yang ber tipe file) ke direktori sedaap menggunakan ```mv```, setelah di ```fork()``` terlebih dahulu

##### d. Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/”harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu3 detik kemudian membuat file bernama “coba2.txt”. (contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).
Jawab : 
```Javascript
snprintf(fileName, sizeof(fileName), "%s/coba1.txt", destName);
pid = fork();
if(pid == 0){
	char * arg[] = {"touch", fileName, NULL};
	execv("/usr/bin/touch", arg);
}
wait(NULL);
sleep(3);
snprintf(fileName, sizeof(fileName), "%s/coba2.txt", destName);
pid = fork();
if(pid == 0){
	char * arg[] = {"touch", fileName, NULL};
	execv("/usr/bin/touch", arg);
}
wait(NULL);
```
-  Apabila de bertipe direktori, maka ada 2 file kosong yang akan dibuat, yaitu file coba1.txt dan coba2.txt.
- Menggunakan fungsi ```snprintf()``` memformat dan menyimpan coba1.txt yang mau disimpan di direktori dalam buffer array.
- Membuat file coba1.txt menggunakan perintah ```touch()```, dan di eksekusi menggunakan perintah ```execv()```
- ```wait(NULL)``` untuk mendelay proses pembuatan file coba2.txt yang merupakan rposes selanjutnya
- Menggunakan perintah ```sleep(3)``` karena file coba2.txt akan dibuat 3 detik setelah coba1.txt dibuat 
- Menggunakan fungsi ```snprintf()``` memformat dan menyimpan coba2.txt yang mau disimpan di direktori dalam buffer array.
- Membuat file coba2.txt menggunakan perintah ```touch()```, dan di eksekusi menggunakan perintah ```execv()```