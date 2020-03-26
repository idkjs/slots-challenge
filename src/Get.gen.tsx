/** 
 * this is a custom header you can add to every file!
 * TypeScript file generated from Get.re by genType.
 */
/* eslint-disable import/first */


// tslint:disable-next-line:no-var-requires
const GetBS = require('./Get.bs');

// tslint:disable-next-line:interface-over-type-literal
export type date = Date;

export interface Iavailability { readonly date: date; readonly slots: string[] };

// tslint:disable-next-line:interface-over-type-literal
export type availabilities = Iavailability[];

export const getAvailabilities: (date:date) => availabilities = GetBS.getAvailabilities;
