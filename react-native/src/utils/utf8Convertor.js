import { TextDecoder, TextEncoder } from 'text-decoding';

export const encode = (string) => {
  const utf8encoder = new TextEncoder('utf-8');
  return utf8encoder.encode(string);
};

export const decode = (arr) => {
  const utf8decoder = new TextDecoder('utf-8');
  const utf8Arr = new Uint8Array(arr);
  return utf8decoder.decode(utf8Arr);
};
