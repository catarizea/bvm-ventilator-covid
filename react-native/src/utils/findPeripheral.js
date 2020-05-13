import get from 'lodash.get';

export default (peripherals, deviceUUID) => {
  let found = false;
  const iterator = peripherals.values();

  for (const item of iterator) {
    const id = get(item, 'id', null);
    if (id && id === deviceUUID) {
      found = item;
    }
  }

  return found;
};
