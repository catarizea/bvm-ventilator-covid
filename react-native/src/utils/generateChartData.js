import { format, add, parse } from 'date-fns';

export default ({ howMany, lastItem }) => {
  const data = [];
  let startDate = new Date();

  if (lastItem && lastItem.x) {
    startDate = parse(lastItem.x, new Date());
  }

  for (let i = 0; i < howMany; i++) {
    const newDate = add(startDate, { days: i });
    data.push({
      x: format(newDate, 'yyyy-MM-dd'),
      y: Math.round(Math.random() * 100),
    });
  }

  return data;
};
