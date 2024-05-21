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
 21;
 -5.89111;-0.00001;-9.59046;,
 5.89111;-0.00001;-9.59046;,
 4.52435;-4.67715;-7.34926;,
 -4.52436;-4.67715;-7.34926;,
 11.12597;-0.00001;-0.00000;,
 9.04871;-4.67715;-0.00000;,
 5.89111;-0.00001;9.59046;,
 4.52436;-4.67715;7.34926;,
 -5.89111;-0.00001;9.59046;,
 -4.52435;-4.67715;7.34926;,
 -11.12597;-0.00001;0.00000;,
 -9.04871;-4.67715;0.00000;,
 -5.89111;-0.00001;-9.59046;,
 -4.52436;-4.67715;-7.34926;,
 11.12597;-0.00001;-0.00000;,
 5.89111;-0.00001;-9.59046;,
 5.89111;-0.00001;9.59046;,
 -5.89111;-0.00001;9.59046;,
 -5.89111;-0.00001;-9.59046;,
 -11.12597;-0.00001;0.00000;,
 0.00000;-8.60562;-0.00000;;
 
 16;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 3;14,15,16;,
 3;16,15,17;,
 3;15,18,17;,
 3;18,19,17;,
 3;3,2,20;,
 3;2,5,20;,
 3;5,7,20;,
 3;7,9,20;,
 3;9,11,20;,
 3;11,13,20;;
 
 MeshMaterialList {
  3;
  16;
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
  0,
  0,
  0,
  0;;
  Material {
   0.097255;0.150588;0.279216;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.008000;0.008000;0.008000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.024000;0.032000;0.064000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  14;
  -0.445674;-0.464813;-0.765064;,
  0.445674;-0.464813;-0.765064;,
  0.894889;-0.446289;-0.000000;,
  0.445674;-0.464813;0.765064;,
  -0.445674;-0.464813;0.765064;,
  -0.894889;-0.446289;0.000000;,
  -0.334588;-0.736991;-0.587278;,
  0.334588;-0.736991;-0.587278;,
  0.676084;-0.736825;-0.000000;,
  0.334588;-0.736991;0.587278;,
  -0.334588;-0.736991;0.587278;,
  -0.676084;-0.736825;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;;
  16;
  4;0,1,7,6;,
  4;1,2,8,7;,
  4;2,3,9,8;,
  4;3,4,10,9;,
  4;4,5,11,10;,
  4;5,0,6,11;,
  3;13,13,13;,
  3;13,13,13;,
  3;13,13,13;,
  3;13,13,13;,
  3;6,7,12;,
  3;7,8,12;,
  3;8,9,12;,
  3;9,10,12;,
  3;10,11,12;,
  3;11,6,12;;
 }
 MeshTextureCoords {
  21;
  0.250000;0.500000;,
  0.333330;0.500000;,
  0.416670;0.250000;,
  0.375000;0.250000;,
  0.416670;0.500000;,
  0.458330;0.250000;,
  0.500000;0.500000;,
  0.500000;0.250000;,
  0.583330;0.500000;,
  0.541670;0.250000;,
  0.666670;0.500000;,
  0.583330;0.250000;,
  0.750000;0.500000;,
  0.625000;0.250000;,
  0.250000;0.750000;,
  0.375000;0.966510;,
  0.375000;0.533490;,
  0.625000;0.533490;,
  0.625000;0.966510;,
  0.750000;0.750000;,
  0.500000;0.000000;;
 }
}
