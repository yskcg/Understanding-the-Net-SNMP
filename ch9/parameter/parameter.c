/*
 * Note: this file originally auto-generated by mib2c using
 *        $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "parameter.h"

/*
 * parameter_variables_oid:
 *   this is the top level oid that we want to register under.  This
 *   is essentially a prefix, with the suffix appearing in the
 *   variable below.
 */

oid             parameter_variables_oid[] =
    { 1, 3, 6, 1, 4, 1, 8072, 9999, 9999, 1, 2 };


/*
 * variable4 parameter_variables:
 *   this variable defines function callbacks and type return information 
 *   for the parameter mib section 
 */

struct variable4 parameter_variables[] = {
    /*
     * magic number        , variable type , ro/rw , callback fn  , L, oidsuffix 
     */
#define PARAMETERA		1
    {PARAMETERA, ASN_INTEGER, NETSNMP_OLDAPI_RWRITE,
     var_parameter, 1, {1}},
#define PARAMETERB		2
    {PARAMETERB, ASN_OCTET_STR, NETSNMP_OLDAPI_RWRITE,
     var_parameter, 1, {2}},

#define PARAMETERINDEX		1
    {PARAMETERINDEX, ASN_INTEGER, NETSNMP_OLDAPI_RONLY,
     var_parameterTable, 3, {3, 1, 1}},
#define PARAMETERC1		2
    {PARAMETERC1, ASN_INTEGER, NETSNMP_OLDAPI_RWRITE,
     var_parameterTable, 3, {3, 1, 2}},
};

/*
 * (L = length of the oidsuffix) 
 */


/** Initializes the parameter module */
void
init_parameter(void)
{

    DEBUGMSGTL(("parameter", "Initializing\n"));

    /*
     * register ourselves with the agent to handle our mib tree 
     */
    REGISTER_MIB("parameter", parameter_variables, variable4,
                 parameter_variables_oid);
	
    /*
     * place any other initialization junk you need here 
     */
     DEBUGMSGTL(("parameter", "init_shm_sem( SLAVE )\n"));
     init_shm_sem_slave();
	
}

/*
 * var_parameter():
 *   This function is called every time the agent gets a request for
 *   a scalar variable that might be found within your mib section
 *   registered above.  It is up to you to do the right thing and
 *   return the correct value.
 *     You should also correct the value of "var_len" if necessary.
 *
 *   Please see the documentation for more information about writing
 *   module extensions, and check out the examples in the examples
 *   and mibII directories.
 */
unsigned char  *
var_parameter(struct variable *vp,
              oid * name,
              size_t *length,
              int exact, size_t *var_len, WriteMethod ** write_method)
{
	static u_char VAR[MAX_CHAR_LEN]={0};

    if (header_generic(vp, name, length, exact, var_len, write_method)
        == MATCH_FAILED)
        return NULL;

    /*
     * this is where we do the value assignments for the mib results.
     */
    switch (vp->magic) {
    case PARAMETERA:
        *write_method = write_parameterA;
	 *var_len = sizeof(long); // 统一使用了u_char VAR	
        snmp_get_data(SHM_PARADATA,PARA_A,sizeof(long),VAR); 
        return (u_char *) &VAR;
		
    case PARAMETERB:
        *write_method = write_parameterB;
        snmp_get_data(SHM_PARADATA,PARA_B,MAX_CHAR_LEN,VAR);
	DEBUGMSGTL(("parameter", "PARAMETERB = %s(%d)\n",VAR,strlen(VAR)));
	*var_len = strlen(VAR) ;//MAX_CHAR_LEN;
        return (u_char *) &VAR;
		
    default:
        ERROR_MSG("NO THIS MAGIC!!");
    }
    return NULL;
}


/*
 * var_parameterTable():
 *   Handle this table separately from the scalar value case.
 *   The workings of this are basically the same as for var_parameter above.
 */
unsigned char  *
var_parameterTable(struct variable *vp,
                   oid * name,
                   size_t *length,
                   int exact, size_t *var_len, WriteMethod ** write_method)
{
	int index = 0;
	static int idx = 0;
	static u_char VAR[MAX_CHAR_LEN]={0};

	/*
     * This assumes that the table is a 'simple' table.
     *  See the implementation documentation for the meaning of this.
     *  You will need to provide the correct value for the TABLE_SIZE parameter
     *
     * If this table does not meet the requirements for a simple table,
     *  you will need to provide the replacement code yourself.
     *  Mib2c is not smart enough to write this for you.
     *    Again, see the implementation documentation for what is required.
     */
    if (header_simple_table
        (vp, name, length, exact, var_len, write_method, C_ROW_NUM )
        == MATCH_FAILED)
        return NULL;

	 index = name[*length - 1] - 1;
	 if( index > C_ROW_NUM )
	 	 return NULL;
	 
	 DEBUGMSGTL(("parameter", "index = %d(%d)\n",index,vp->magic));

    /*
     * this is where we do the value assignments for the mib results.
     */
    switch (vp->magic) {
    case PARAMETERINDEX:
		idx = index + 1;
		DEBUGMSGTL(("parameter", "--2index = %d\n",idx));
        return (u_char *) &(idx);
	 //return NULL; // 直接返回NULL或break,后台就查不到索引数据了，也就是不能选择查看第几行数据，而只能walk
	 
    case PARAMETERC1:
        *write_method = write_parameterC1;  
	 snmp_get_data(SHM_PARADATA,index+C_OFFSET,sizeof(long),VAR); 
        return (u_char *) &VAR;
    default:
        ERROR_MSG("");
    }
    return NULL;
}



int
write_parameterA(int action,
                 u_char * var_val,
                 u_char var_val_type,
                 size_t var_val_len,
                 u_char * statP, oid * name, size_t name_len)
{
	    long            value;

    switch (action) {
    case RESERVE1:
        if (var_val_type != ASN_INTEGER) {
            fprintf(stderr, "write to parameter not ASN_INTEGER\n");
            return SNMP_ERR_WRONGTYPE;
        }
        if (var_val_len > sizeof(long)) {
            fprintf(stderr, "write to parameter: bad length\n");
            return SNMP_ERR_WRONGLENGTH;
        }
        break;

    case RESERVE2:
//	        size = var_val_len;
	        value = *(long *) var_val;
        break;

    case FREE:
        /*
         * Release any resources that have been allocated 
         */
        break;

    case ACTION:
        /*
         * The variable has been stored in 'value' for you to use,
         * and you have just been asked to do something with it.
         * Note that anything done here must be reversable in the UNDO case
         */
         if(snmp_set_data(SHM_PARADATA,PARA_A,var_val_len,&value) <= 0)  
			return SNMP_ERR_WRONGVALUE;
        break;

    case UNDO:
        /*
         * Back out any changes made in the ACTION case 
         */
        break;

    case COMMIT:
        /*
         * Things are working well, so it's now safe to make the change
         * permanently.  Make sure that anything done here can't fail!
         */
        break;
    }
    return SNMP_ERR_NOERROR;
}


int
write_parameterB(int action,
                 u_char * var_val,
                 u_char var_val_type,
                 size_t var_val_len,
                 u_char * statP, oid * name, size_t name_len)
{
    static u_char            value[MAX_CHAR_LEN];

    switch (action) {
    case RESERVE1:
        if (var_val_type != ASN_OCTET_STR) {
            fprintf(stderr, "write to parameter not ASN_OCTET_STR\n");
            return SNMP_ERR_WRONGTYPE;
        }
        if (var_val_len > MAX_CHAR_LEN ) {
            fprintf(stderr, "write to parameter: bad length\n");
            return SNMP_ERR_WRONGLENGTH;
        }
        break;

    case RESERVE2:
        //size = var_val_len;
        //value = *(char *) var_val;
        DEBUGMSGTL(("parameter", "PARAMETERB = %s(%d,%d)\n",var_val,var_val_len,strlen(var_val)));
		strncpy(value, var_val,var_val_len);
		value[var_val_len]='\0';
		DEBUGMSGTL(("parameter", "--value = %s\n",value));

        break;

    case FREE:
        /*
         * Release any resources that have been allocated 
         */
        break;

    case ACTION:
        /*
         * The variable has been stored in 'value' for you to use,
         * and you have just been asked to do something with it.
         * Note that anything done here must be reversable in the UNDO case
         */
         if(snmp_set_data(SHM_PARADATA,PARA_B,var_val_len+1,value)<=0)  // 或 strlen(value)+1
			return SNMP_ERR_WRONGVALUE;
        break;

    case UNDO:
        /*
         * Back out any changes made in the ACTION case 
         */
        break;

    case COMMIT:
        /*
         * Things are working well, so it's now safe to make the change
         * permanently.  Make sure that anything done here can't fail!
         */
        break;
    }
    return SNMP_ERR_NOERROR;
}
// 该函数只写 parameterC1 一列,所以 magic 都不需要使用，否则需要使用通过
// oid * name 找到 magic
int
write_parameterC1(int action,
                  u_char * var_val,
                  u_char var_val_type,
                  size_t var_val_len,
                  u_char * statP, oid * name, size_t name_len)
{
    long            value;
	static tt =0;
	DEBUGMSGTL(("parameter", "action = %d(%d)\n",action,tt++));

    switch (action) {
    case RESERVE1:
        if (var_val_type != ASN_INTEGER) {
            fprintf(stderr, "write to parameter not ASN_INTEGER\n");
            return SNMP_ERR_WRONGTYPE;
        }
        if (var_val_len > sizeof(long)) {
            fprintf(stderr, "write to parameter: bad length\n");
            return SNMP_ERR_WRONGLENGTH;
        }
        break;

    case RESERVE2:
        //size = var_val_len;
        value = *(long *) var_val;

        break;

    case FREE:
        /*
         * Release any resources that have been allocated 
         */
        break;

    case ACTION:
        /*
         * The variable has been stored in 'value' for you to use,
         * and you have just been asked to do something with it.
         * Note that anything done here must be reversable in the UNDO case
         */
         if( snmp_set_data(SHM_PARADATA,name[name_len-1]+C_OFFSET-1,var_val_len,&value) <=0 )  
			return SNMP_ERR_WRONGVALUE; 
        break;

    case UNDO:
        /*
         * Back out any changes made in the ACTION case 
         */
        break;

    case COMMIT:
        /*
         * Things are working well, so it's now safe to make the change
         * permanently.  Make sure that anything done here can't fail!
         */
         tt=0;
        break;
    }
    return SNMP_ERR_NOERROR;
}
