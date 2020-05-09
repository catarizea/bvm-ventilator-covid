import React from 'react';
import { View, StyleSheet } from 'react-native';

import ScreenContainer from '../components/ScreenContainer';

const ChartsScreen = () => {
  return (
    <ScreenContainer>
      <View style={styles.main} />
    </ScreenContainer>
  );
};

const styles = StyleSheet.create({
  main: {
    flex: 1,
  },
});

export default ChartsScreen;
