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
 14;
 -2.51081;12.61918;-5.58747;,
 -3.58047;11.90204;7.50515;,
 -3.37507;16.66568;2.41499;,
 -4.75478;-0.07752;7.19709;,
 -3.78517;2.68684;-7.90097;,
 2.72838;13.32043;0.93799;,
 6.42368;4.71028;-0.14325;,
 -2.51081;12.61918;-5.58747;,
 -3.37507;16.66568;2.41499;,
 -3.58047;11.90204;7.50515;,
 -4.75478;-0.07752;7.19709;,
 -3.78517;2.68684;-7.90097;,
 2.72838;13.32043;0.93799;,
 6.42368;4.71028;-0.14325;;
 
 18;
 3;0,1,2;,
 3;0,3,1;,
 3;0,4,3;,
 3;5,0,2;,
 3;5,2,1;,
 3;6,0,5;,
 3;6,4,0;,
 3;6,1,3;,
 3;6,5,1;,
 3;7,8,9;,
 3;7,9,10;,
 3;7,10,11;,
 3;12,8,7;,
 3;12,9,8;,
 3;13,12,7;,
 3;13,7,11;,
 3;13,10,9;,
 3;13,9,12;;
 
 MeshMaterialList {
  1;
  18;
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
  0,
  0,
  0;;
  Material {
   0.121569;0.188235;0.349020;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  30;
  -0.995807;0.063048;-0.066279;,
  -0.995380;-0.046711;-0.083881;,
  -0.996450;0.026317;-0.079968;,
  -0.991408;0.117654;-0.057168;,
  -0.989951;0.136022;-0.038671;,
  0.562768;0.641903;-0.520819;,
  0.654710;0.248118;-0.713998;,
  0.595974;0.479140;-0.644391;,
  0.369292;0.844770;-0.387281;,
  0.603790;0.428020;0.672485;,
  0.490051;0.626503;0.606089;,
  0.611662;0.264831;0.745476;,
  0.587174;0.111413;-0.801757;,
  0.635159;0.065036;0.769639;,
  0.570074;-0.076917;0.817985;,
  0.995807;-0.063048;0.066279;,
  0.995380;0.046711;0.083881;,
  0.996450;-0.026317;0.079968;,
  0.991408;-0.117654;0.057168;,
  0.989951;-0.136022;0.038671;,
  -0.562768;-0.641903;0.520819;,
  -0.654710;-0.248118;0.713998;,
  -0.369292;-0.844770;0.387281;,
  -0.595974;-0.479140;0.644391;,
  -0.603790;-0.428020;-0.672485;,
  -0.611662;-0.264831;-0.745476;,
  -0.490051;-0.626503;-0.606089;,
  -0.587174;-0.111413;0.801757;,
  -0.635159;-0.065036;-0.769639;,
  -0.570074;0.076917;-0.817985;;
  18;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;5,7,8;,
  3;9,10,11;,
  3;6,7,5;,
  3;6,12,7;,
  3;13,11,14;,
  3;13,9,11;,
  3;15,16,17;,
  3;15,17,18;,
  3;15,18,19;,
  3;20,22,23;,
  3;24,25,26;,
  3;21,20,23;,
  3;21,23,27;,
  3;28,29,25;,
  3;28,25,24;;
 }
 MeshTextureCoords {
  14;
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
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
