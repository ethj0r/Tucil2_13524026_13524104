# Voxelization Objek 3D Menggunakan Octree

## Deskripsi Program

Program ini mengubah model 3D berbentuk mesh (*file OBJ*) menjadi representasi voxel (*kubus kecil*)  menggunakan struktur data octree. Setiap voxel merupakan bounding box berbentuk kubus yang menandai area di mana model 3D tersebut ada.

## Teori Singkat

### Voxelization

Voxelization adalah proses membagi model 3D menjadi unit-unit kubus kecil yang disebut voxel. Sebagai analogi, bayangkan model patung dibagi menjadi banyak kotak kecil berukuran sama. Selanjutnya, ditentukan kotak mana yang berpotongan dengan permukaan model.

Setiap unit voxel diberi status:
- **Kosong**: Tidak menyentuh objek model
- **Terisi**: Berisi bagian dari permukaan atau volume model

Hasil akhirnya adalah model 3D yang tidak lagi halus, tetapi terdiri dari kumpulan kubus-kubus diskrit.

### Struktur Octree

Octree adalah struktur hierarki untuk membagi ruang 3D secara efisien dalam proses voxelization. Prinsip kerjanya sebagai berikut:

1. **Level 1**: Ruang dibagi menjadi 8 subregion yang sama (*struktur 2×2×2*)
2. **Level 2**: Setiap subregion yang memotong model dibagi lagi menjadi 8 bagian lebih kecil
3. **Level 3**: Pembagian berlanjut sampai mencapai kedalaman maksimum yang ditentukan
4. **Optimasi**: Subregion yang kosong (tidak memotong model) tidak dibagi lebih lanjut (*pruning*)

Dalam program ini, octree dibangun secara rekursif dengan:
1. Membagi bounding box menjadi 8 bagian
2. Menguji bagian mana yang berpotongan dengan segitiga
3. Rekursif membagi bagian tersebut yang berpotongan hingga kedalaman maksimum
4. Menyimpan node daun sebagai voxel

**Keuntungan:**
- **Efisiensi Memori**: Hanya menyimpan region yang berpotongan dengan model, menghemat ruang penyimpanan
- **Adaptif**: Area dengan geometri kompleks dibagi lebih banyak tingkat, sedangkan area sederhana tidak perlu dibagi banyak
- **Akses Cepat**: Struktur hierarki memungkinkan pencarian dan akses data lebih cepat

## Proses Kerja:
1. Membaca file model 3D dalam format OBJ
2. Membagi ruang 3D menjadi grid hierarki menggunakan octree
3. Menentukan voxel mana yang berpotongan dengan permukaan model (segitiga)
4. Menyimpan hasil voxel sebagai file OBJ baru dan menampilkannya secara visual

## Persyaratan

### Perangkat Lunak
- **C++ Compiler**: C++17 atau lebih baru (clang++ atau g++)
- **Build Tool**: GNU Make
- **pkg-config**: Untuk manajemen library
- **Raylib**: Library grafis untuk visualisasi 3D (v4.0+)

### Dependensi Sistem

**Windows (MSYS2/MinGW)**:
```bash
pacman -S mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-raylib
pacman -S mingw-w64-x86_64-pkg-config
```

**Linux (Ubuntu/Debian)**:
```bash
sudo apt-get install build-essential pkg-config
sudo apt-get install libraylib-dev
```

**macOS**:
```bash
brew install raylib pkg-config
```

## Cara Kompilasi dan Menjalankan

### Kompilasi

Navigasi ke direktori `src` dan jalankan:
```bash
cd src
make clean
make run
```

### Input

Ketika program dimulai, akan diminta untuk memasukkan:

1. **Path ke file .obj**: Masukkan path relatif atau absolut ke file model 3D dalam format OBJ
   - Contoh: `../test/cube.obj`

2. **Kedalaman maksimum octree**: Masukkan level kedalaman maksimum untuk octree (*min 1*)
   - Nilai lebih besar = voxel lebih halus, tetapi lebih mahal secara komputasi (*membutuhkan waktu yang lebih lama*)

### Output

- **Model Voxelisasi**: Disimpan sebagai `output.obj` dalam folder `test`  yang berisi representasi voxel sebagai mesh
- **Viewer 3D**: Visualisasi interaktif terbuka secara otomatis menampilkan model voxelisasi
- **Statistik**: Output console menampilkan:
  - Jumlah vertex dan segitiga input
  - Statistik octree (jumlah node per level kedalaman, node yang dipangkas)
  - Waktu eksekusi (dalam milidetik)
  - Total jumlah voxel

## Struktur Proyek

```
.
├── src/
│   ├── main.cpp                    
│   ├── Makefile                    
│   ├── model/                      # Struktur data
│   │   ├── Mesh.{cpp,hpp}         
│   │   ├── Triangle.{cpp,hpp}     
│   │   ├── Vector.{cpp,hpp}       
│   │   └── BoundingBox.{cpp,hpp}  
│   ├── parser/                     # Parsing file
│   │   └── ObjParser.{cpp,hpp}    
│   ├── octree/                     # Algoritma utama
│   │   └── Octree.{cpp,hpp}       
│   ├── exporter/                   # Export ke format OBJ
│   │   └── ObjExporter.{cpp,hpp}  
│   ├── viewer/                     # Visualisasi
│   │   ├── Viewer.{cpp,hpp}       
│   │   └── fonts/                 
│   └── stats/                      # Statistik
│       └── stats.{cpp,hpp}       
├── bin/                            
├── test/                           
├── doc/                          
│   └── laporan.tex                
└── README.md                       
```

## Contoh Output

```
Path ke .obj file: ../test/cow.obj
Max depth octree: 4

Mesh loaded: 8 vertices, 12 triangles

VOXELIZATION REPORT
Voxel count    : 1352
Vertex count   : 10816
Face count     : 16224

Octree nodes per depth:
  0 : 1
  1 : 8
  2 : 64
  3 : 448
  4 : 2368

Pruned nodes per depth:
  0 : 0
  1 : 0
  2 : 8
  3 : 152
  4 : 1016

Octree depth   : 4
Execution time : 3.43505 ms
```


## Catatan

- Program menggunakan multithreading selama konstruksi octree untuk performa yang lebih baik
- Bagian sub-kubus yang kosong dipangkas untuk mengoptimalkan penggunaan memori
- Viewer dapat dinavigasi dengan kontrol mouse dan keyboard
