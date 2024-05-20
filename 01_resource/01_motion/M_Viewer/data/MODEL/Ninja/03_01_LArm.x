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
 12;
 -4.74001;1.12525;-1.84325;,
 2.69449;13.03415;-2.78747;,
 -2.03115;1.10181;-5.60097;,
 -2.04471;13.73541;1.73799;,
 4.26415;11.31701;4.80515;,
 -0.06155;-0.16255;-0.00291;,
 -4.74001;1.12525;-1.84325;,
 -2.03115;1.10181;-5.60097;,
 2.69449;13.03415;-2.78747;,
 -2.04471;13.73541;1.73799;,
 4.26415;11.31701;4.80515;,
 -0.06155;-0.16255;-0.00291;;
 
 12;
 3;0,1,2;,
 3;0,3,1;,
 3;0,4,3;,
 3;0,5,4;,
 3;5,4,1;,
 3;5,1,2;,
 3;6,7,8;,
 3;6,8,9;,
 3;6,9,10;,
 3;6,10,11;,
 3;11,8,10;,
 3;11,7,8;;
 
 MeshMaterialList {
  1;
  12;
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
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  16;
  -0.684535;0.377917;-0.623371;,
  -0.918408;0.385103;-0.090677;,
  -0.914859;0.362864;-0.177094;,
  -0.624637;0.333981;-0.705893;,
  -0.672151;-0.041890;0.739228;,
  -0.823785;0.108854;0.556353;,
  -0.443272;-0.181067;0.877909;,
  -0.476855;-0.144317;0.867054;,
  0.684535;-0.377917;0.623371;,
  0.918408;-0.385103;0.090677;,
  0.914859;-0.362864;0.177094;,
  0.624637;-0.333981;0.705893;,
  0.672151;0.041890;-0.739228;,
  0.823785;-0.108854;-0.556353;,
  0.443272;0.181067;-0.877909;,
  0.476855;0.144317;-0.867054;;
  12;
  3;0,2,1;,
  3;0,3,2;,
  3;6,4,7;,
  3;6,5,4;,
  3;5,4,2;,
  3;5,2,1;,
  3;8,9,10;,
  3;8,10,11;,
  3;14,15,12;,
  3;14,12,13;,
  3;13,10,12;,
  3;13,9,10;;
 }
 MeshTextureCoords {
  12;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
