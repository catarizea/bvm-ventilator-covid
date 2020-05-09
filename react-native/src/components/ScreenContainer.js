import React from 'react';
import { SafeAreaView, StyleSheet } from 'react-native';
import PropTypes from 'prop-types';

const ScreenContainer = (props) => {
  const { children } = props;

  return <SafeAreaView style={styles.container}>{children}</SafeAreaView>;
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#17204d',
  },
});

ScreenContainer.propTypes = {
  children: PropTypes.element.isRequired,
};

export default ScreenContainer;
