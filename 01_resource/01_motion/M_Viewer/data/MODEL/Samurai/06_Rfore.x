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
 0.26269;-3.12420;-1.97137;,
 -0.38257;1.25469;-1.97137;,
 -0.38257;1.25469;2.45481;,
 0.26269;-3.12420;2.45481;,
 0.26269;-3.12420;2.45481;,
 -0.38257;1.25469;2.45481;,
 -10.50429;-4.04772;1.09169;,
 -10.26440;-5.62005;1.09169;,
 -10.26440;-5.62005;-0.60825;,
 -10.50429;-4.04772;-0.60825;,
 -0.38257;1.25469;-1.97137;,
 0.26269;-3.12420;-1.97137;,
 -10.26440;-5.62005;-0.60825;,
 0.26269;-3.12420;-1.97137;;
 
 5;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;4,7,12,13;,
 4;6,5,10,9;;
 
 MeshMaterialList {
  2;
  5;
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
    "data\\TEXTURE\\MODEL\\samurai_Tex_New.png";
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
  5;
  0.989317;0.145783;0.000000;,
  -0.123926;-0.018584;-0.992117;,
  -0.123926;-0.018584;0.992117;,
  0.230694;-0.973026;-0.000000;,
  -0.464045;0.885812;0.000000;;
  5;
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;4,4,4,4;;
 }
 MeshTextureCoords {
  14;
  0.485800;0.354440;,
  0.485800;0.311250;,
  0.442610;0.311250;,
  0.442610;0.354440;,
  0.490060;0.461500;,
  0.488360;0.418340;,
  0.383810;0.459290;,
  0.384500;0.474800;,
  0.363450;0.435510;,
  0.375970;0.444680;,
  0.467950;0.380280;,
  0.432960;0.354970;,
  0.384500;0.491380;,
  0.490060;0.504690;;
 }
}
