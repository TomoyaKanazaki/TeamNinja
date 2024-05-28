xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 24;
 -1.48657;-14.24362;-2.35509;,
 -1.30555;-17.04288;1.99851;,
 -3.22823;-0.81290;3.27199;,
 -3.22823;-0.81290;-2.82199;,
 -1.30555;-17.04288;1.99851;,
 0.94305;-17.04288;1.99851;,
 2.86575;-0.81290;3.27199;,
 -3.22823;-0.81290;3.27199;,
 0.94305;-17.04288;1.99851;,
 0.98173;-14.24362;-2.35509;,
 2.86575;-0.81290;-2.82199;,
 2.86575;-0.81290;3.27199;,
 2.86575;-0.81290;-2.82199;,
 -3.22823;-0.81290;-2.82199;,
 1.93191;-17.57110;-1.88817;,
 0.95177;-17.41254;-8.07461;,
 1.20213;-15.93385;-8.00815;,
 -1.31427;-17.41254;-8.07461;,
 -1.56461;-15.93385;-8.00815;,
 -2.29441;-17.57110;-1.88817;,
 -2.29441;-17.57110;-1.88817;,
 -1.31427;-17.41254;-8.07461;,
 0.95177;-17.41254;-8.07461;,
 1.93191;-17.57110;-1.88817;;
 
 13;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;9,0,3,10;,
 4;6,12,13,7;,
 4;14,15,16,9;,
 4;15,17,18,16;,
 4;9,16,18,0;,
 4;0,18,17,19;,
 4;20,21,22,23;,
 3;9,8,14;,
 3;0,19,1;,
 4;23,5,4,20;;
 
 MeshMaterialList {
  2;
  13;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\TeamNinja\\01_resource\\00_model\\Enemy\\Samurai\\source\\samurai_Tex_New.png";
   }
  }
  Material {
   0.416000;0.320000;0.064000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  18;
  -0.987497;0.149806;0.049077;,
  -0.992415;-0.121844;-0.016349;,
  0.982634;0.179184;0.048213;,
  0.991549;-0.125778;-0.031778;,
  0.000000;-0.078224;0.996936;,
  -0.997910;0.058610;0.027227;,
  -0.992854;0.069805;0.096793;,
  0.996794;0.077088;0.021447;,
  0.993034;0.056839;-0.103209;,
  0.000000;0.044900;-0.998991;,
  0.992526;0.087041;0.085538;,
  0.000000;-0.034743;-0.999396;,
  0.000000;1.000000;0.000000;,
  0.000000;0.958091;-0.286465;,
  -0.993409;0.035930;-0.108851;,
  0.000000;-0.998503;0.054699;,
  0.000000;-0.999672;-0.025622;,
  0.000000;-0.990891;0.134667;;
  13;
  4;5,6,1,1;,
  4;4,4,4,4;,
  4;10,7,3,3;,
  4;11,11,11,11;,
  4;12,12,12,12;,
  4;2,8,8,7;,
  4;9,9,9,9;,
  4;13,13,13,13;,
  4;5,14,14,0;,
  4;15,16,16,15;,
  3;7,10,2;,
  3;5,0,6;,
  4;15,17,17,15;;
 }
 MeshTextureCoords {
  24;
  0.836160;0.570900;,
  0.891610;0.563360;,
  0.891610;0.656100;,
  0.846310;0.656550;,
  0.865850;0.385380;,
  0.843910;0.385380;,
  0.825150;0.285180;,
  0.884610;0.285180;,
  0.748730;0.554370;,
  0.804830;0.568270;,
  0.795110;0.656780;,
  0.744460;0.655350;,
  0.825150;0.225720;,
  0.884610;0.225720;,
  0.778650;0.536320;,
  0.815660;0.507150;,
  0.817420;0.517060;,
  0.834440;0.507750;,
  0.830850;0.519260;,
  0.866440;0.544320;,
  0.875500;0.423650;,
  0.865940;0.484030;,
  0.843830;0.484030;,
  0.834260;0.423650;;
 }
}
